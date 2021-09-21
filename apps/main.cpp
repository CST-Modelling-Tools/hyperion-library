#include <iostream>
#include <chrono>
#include <cstdlib>

#include "idealefficiencymap.h"
#include "auxfunction.h"

int main(int, char**) {


    //Set the environment
    double latitude = -12.6 * hypl::mathconstants::degree;
    hypl::Location location(latitude);
    double env_io {700.26};
    double env_beta {0.112};
    auto att_model = hypl::Atmosphere::AttenuationModel::SW;
    hypl::Atmosphere atmosphere(env_io,env_beta,att_model);
    hypl::Environment environment(location, atmosphere, 1);

    // Multiple Receivers
    // std::vector<hypl::Receiver> receivers;
    // double rcv_radius = 0.;
    // hypl::vec3d aiming_point[3] { hypl::vec3d(-100.0, 0.0, 50.0),
    //                               hypl::vec3d(   0.0, 0.0, 100.0),
    //                               hypl::vec3d( 50.0, 80.0, 75.0) };
    // for (auto& element : aiming_point)
    //     receivers.push_back(hypl::Receiver(element,rcv_radius));

    // One Receiver
    std::vector<hypl::Receiver> receivers;
    double rcv_radius = 5.;
    receivers.push_back(hypl::Receiver({ hypl::vec3d(100.0, 0.0, 100.0)},rcv_radius));

    //Set the field
    hypl::Boundaries boundaries;//(-500.,500.,-500.,500);
    auto start_time = std::chrono::high_resolution_clock::now();
    hypl::IdealEfficiencyMap heliostatfield(environment, boundaries, receivers, 1000, 1000);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Construction running time: " << duration.count()/60000.0 << " minutes\n";
    std::cout << "Nber of Receivers: " << heliostatfield.receivers().size() << std::endl;


    //Check rcv update
    std::cout << "Helio 5\n";
    rcv_radius = heliostatfield.heliostats().at(5)->receivers().at(0).radius();
    std::cout << "Receiver radius " << rcv_radius << std::endl;
    std::cout << "Helio 1\n";
    rcv_radius = heliostatfield.heliostats().at(1)->receivers().at(0).radius();
    std::cout << "Receiver radius " << rcv_radius << std::endl;

    int day = 173;
    double angle = environment.get_sun_subtended_angle(day+11); //day difference in numbering
    std::cout << "Day number: " << day+11 << ",  Sun Subtended angle: " << angle << ", Declination: " << hypl::auxfunction::SolarDeclinationByDayNumber(day) << std::endl;
    day = 356;
    angle = environment.get_sun_subtended_angle(day+11); //day difference in numbering
    std::cout << "Day number: " << day+11 << ",  Sun Subtended angle: " << angle << ", Declination: " << hypl::auxfunction::SolarDeclinationByDayNumber(day) << std::endl;

    //Calculate field efficiency
    start_time = std::chrono::high_resolution_clock::now();
    heliostatfield.EvaluateAnnualEfficiencies();
    double fieldefficiency=0.;
    for (auto& element : heliostatfield.heliostats())
        fieldefficiency += element->annual_ideal_efficiency();

    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Efficiency evaluation running time: " << duration.count()/60000.0 << " minutes\n";
    std::cout << "Accumulated efficiency of the field " << fieldefficiency << ", Efficiency: " << fieldefficiency/heliostatfield.heliostats().size() << std::endl;
    return 0;
}