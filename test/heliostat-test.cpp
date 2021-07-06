#include <gtest/gtest.h>

#include "heliostat.h"

/*
        Heliostat(Environment& environment, std::vector<Receiver>& receivers, vec3d center);

        //Accessors
        const Environment& environment() const { return m_environment; }
        const std::vector<Receiver>& receivers() const { return m_receivers; }
        const vec3d& center() const { return m_center; }
        const std::vector<double>& attenuation() const { return m_attenuation; }
        const std::vector<double>& slant_range() const { return m_slant_range; }
        const std::vector<vec3d>& reflected_unit_vector() const { return m_reflected_unit_vector; }
        const double annual_energy_per_unit_area();
        const double annual_ideal_efficiency();
*/


TEST(Heliostat, ConstructorANDBasicAccessors)
{
    double expected_latitude = 28.7 * hypl::mathconstants::degree;
    hypl::Location expected_location(expected_latitude);

    double expected_io {1005.26};
    double expected_beta {0.112};
    auto expected_attenuation_model = hypl::Atmosphere::AttenuationModel::SW;
    hypl::Atmosphere expected_atmosphere(expected_io, expected_beta, expected_attenuation_model);

    int expected_delta_days = 1;
    hypl::Environment expected_environment(expected_location, expected_atmosphere, expected_delta_days);

    std::vector<hypl::Receiver> expected_receivers;

    hypl::vec3d expected_aiming_point[3] { hypl::vec3d(-400.0, 0.0, 100.0), 
                                           hypl::vec3d(   0.0, 0.0, 100.0), 
                                           hypl::vec3d( 400.0, 0.0, 100.0) };

    for( auto element : expected_aiming_point)
        expected_receivers.push_back(hypl::Receiver(element));

    auto expected_heliostat_center = hypl::vec3d(-20.45, 123.78, 0.0);

    hypl::Heliostat heliostat(expected_environment, expected_receivers, expected_heliostat_center);

    auto computed_environment = heliostat.environment();
    auto computed_location = computed_environment.location();
    auto computed_atmosphere = computed_environment.atmosphere();
    int computed_delta_days = computed_environment.delta_days();

    double computed_latitude = computed_location.latitude();
    double computed_io = computed_atmosphere.io();
    double computed_beta = computed_atmosphere.beta();
    auto computed_attenuation_model = computed_atmosphere.attenuation_model();

    auto computed_receivers = heliostat.receivers();

    hypl::vec3d computed_aiming_point[3] = { computed_receivers[0].aiming_point(), 
                                             computed_receivers[1].aiming_point(), 
                                             computed_receivers[2].aiming_point() };

    auto computed_heliostat_center = heliostat.center();

    EXPECT_DOUBLE_EQ(computed_latitude, expected_latitude);
    EXPECT_DOUBLE_EQ(computed_io, expected_io);
    EXPECT_DOUBLE_EQ(computed_beta, expected_beta);
    EXPECT_DOUBLE_EQ(computed_attenuation_model, expected_attenuation_model);

    for( int i = 0; i < 3; i++)
    {
        EXPECT_DOUBLE_EQ(computed_aiming_point[i].x, expected_aiming_point[i].x);
        EXPECT_DOUBLE_EQ(computed_aiming_point[i].y, expected_aiming_point[i].y);
        EXPECT_DOUBLE_EQ(computed_aiming_point[i].z, expected_aiming_point[i].z);
    }

    EXPECT_DOUBLE_EQ(computed_heliostat_center.x, expected_heliostat_center.x);
    EXPECT_DOUBLE_EQ(computed_heliostat_center.y, expected_heliostat_center.y);
    EXPECT_DOUBLE_EQ(computed_heliostat_center.z, expected_heliostat_center.z);
}

TEST(Heliostat, NoneBasicAccessors)
{
    double latitude = -12.89 * hypl::mathconstants::degree;
    hypl::Location location(latitude);

    double io {1017.29};
    double beta {0.114};
    auto attenuation_model = hypl::Atmosphere::AttenuationModel::LH;
    hypl::Atmosphere atmosphere(io, beta, attenuation_model);

    int delta_days = 3;
    hypl::Environment environment(location, atmosphere, delta_days);

    std::vector<hypl::Receiver> receivers;

    hypl::vec3d aiming_point[3] { hypl::vec3d(-283.0, 0.0, 200.0), 
                                  hypl::vec3d(   0.0, -1000.0, 250.0), 
                                  hypl::vec3d( 400.0, 0.0, 90.0) };

    for( auto element : aiming_point)
        receivers.push_back(hypl::Receiver(element));

    auto heliostat_center = hypl::vec3d(202.45, 289.78, 0.0);

    hypl::Heliostat heliostat(environment, receivers, heliostat_center);

    EXPECT_TRUE(true);
}