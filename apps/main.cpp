#include <iostream>
#include <chrono>
#include <cstdlib>

#include "idealefficiencymap.h"

int main(int, char**) {
    

    //Set the environment
    double latitude = -12.6 * hypl::mathconstants::degree;
    hypl::Location location(latitude);
    double env_io {700.26};
    double env_beta {0.112};
    auto att_model = hypl::Atmosphere::AttenuationModel::SW;
    hypl::Atmosphere atmosphere(env_io,env_beta,att_model);
    int delta_days = 1;
    hypl::Environment environment(location, atmosphere, delta_days);

    std::cout << "Beta value: " << environment.atmosphere().beta() << std::endl;

    // Receivers
    std::vector<hypl::Receiver> receivers;
    double rcv_radius = 20.;
    hypl::vec3d aiming_point[3] { hypl::vec3d(-100.0, 0.0, 50.0), 
                                  hypl::vec3d(   0.0, 0.0, 100.0), 
                                  hypl::vec3d( 50.0, 80.0, 75.0) };
    for (auto& element : aiming_point)
        receivers.push_back(hypl::Receiver(element,rcv_radius));
    
    std::cout << "Nber of receivers: " << receivers.size() << std::endl;

    //Set the field
    hypl::Boundaries boundaries(-150.,150.,-100.,200);
    auto start_time = std::chrono::high_resolution_clock::now();
    hypl::IdealEfficiencyMap heliostatfield(environment, boundaries, receivers, 100, 100);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Construction running time: " << duration.count()/60000.0 << " minutes\n";


    //Check rcv update
    std::cout << "Helio 5\n";
    rcv_radius = heliostatfield.heliostats().at(5)->receivers().at(0).radius();
    std::cout << "Initial receiver radius " << rcv_radius << std::endl;
    double rcv_limit = heliostatfield.heliostats().at(5)->shading_limit().at(0);
    std::cout << "Receiver Shadow Limit: " << rcv_limit << std::endl;
    double rcv_angle = heliostatfield.heliostats().at(5)->receiver_subtended_angle().at(0);
    std::cout << "Receiver subtended angle: " << rcv_angle << std::endl;
    std::cout << "Helio 1\n";
    rcv_radius = heliostatfield.heliostats().at(1)->receivers().at(0).radius();
    std::cout << "Initial receiver radius " << rcv_radius << std::endl;
    rcv_limit = heliostatfield.heliostats().at(1)->shading_limit().at(0);
    std::cout << "Receiver Shadow Limit: " << rcv_limit << std::endl;
    rcv_angle = heliostatfield.heliostats().at(1)->receiver_subtended_angle().at(0);
    std::cout << "Receiver subtended angle: " << rcv_angle << std::endl;

    //Check rcv update
    heliostatfield.set_receivers_radius(10.6);
    std::cout << "Helio 5\n";
    rcv_radius = heliostatfield.heliostats().at(5)->receivers().at(0).radius();
    std::cout << "Initial receiver radius " << rcv_radius << std::endl;
    rcv_limit = heliostatfield.heliostats().at(5)->shading_limit().at(0);
    std::cout << "Receiver Shadow Limit: " << rcv_limit << std::endl;
    rcv_angle = heliostatfield.heliostats().at(5)->receiver_subtended_angle().at(0);
    std::cout << "Receiver subtended angle: " << rcv_angle << std::endl;
    std::cout << "Helio 1\n";
    rcv_radius = heliostatfield.heliostats().at(1)->receivers().at(0).radius();
    std::cout << "Initial receiver radius " << rcv_radius << std::endl;
    rcv_limit = heliostatfield.heliostats().at(1)->shading_limit().at(0);
    std::cout << "Receiver Shadow Limit: " << rcv_limit << std::endl;
    rcv_angle = heliostatfield.heliostats().at(1)->receiver_subtended_angle().at(0);
    std::cout << "Receiver subtended angle: " << rcv_angle << std::endl;

    //Calculate field efficiency
    start_time = std::chrono::high_resolution_clock::now();
    heliostatfield.EvaluateAnnualEfficiencies();
    double fieldefficiency=0.;
    for (auto& element : heliostatfield.heliostats())
        fieldefficiency += element->annual_ideal_efficiency();

    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Efficiency evaluation running time: " << duration.count()/60000.0 << " minutes\n";
    std::cout << "Average efficiency of the field " << fieldefficiency << std::endl;
    return 0;
}