#include <gtest/gtest.h>
#include <math.h>
#include "mathconstants.h"
#include "atmosphere.h"
#include "location.h"
#include "boundaries.h"
#include "environment.h"
#include "auxfunction.h"

TEST(AtmosphereTest, DefaultConstructor)
{
    hypl::Atmosphere atmosphere;

    EXPECT_DOUBLE_EQ(atmosphere.io(), 1110.0);
    EXPECT_DOUBLE_EQ(atmosphere.beta(), 0.11);
    EXPECT_EQ(atmosphere.transmittance_model(), hypl::Atmosphere::TransmittanceModel::VB);
}

TEST(AtmosphereTest, Constructor)
{
    double io = 1120.0;
    double beta = 0.10;
    hypl::Atmosphere::TransmittanceModel transmittance_model = hypl::Atmosphere::TransmittanceModel::LH;

    hypl::Atmosphere atmosphere(io, beta, transmittance_model);

    EXPECT_DOUBLE_EQ(atmosphere.io(), io);
    EXPECT_DOUBLE_EQ(atmosphere.beta(), beta);
    EXPECT_EQ(atmosphere.transmittance_model(), transmittance_model);
}

TEST(AtmosphereTest, MutatorsAndAccessors)
{
    hypl::Atmosphere atmosphere;
    double io = 1110.0;
    double beta = 0.11;
    hypl::Atmosphere::TransmittanceModel transmittance_model = hypl::Atmosphere::TransmittanceModel::VB;

    EXPECT_DOUBLE_EQ(atmosphere.io(), io);
    EXPECT_DOUBLE_EQ(atmosphere.beta(), beta);
    EXPECT_EQ(atmosphere.transmittance_model(), transmittance_model);

    io = 2120.0;
    beta = 0.20;
    transmittance_model = hypl::Atmosphere::TransmittanceModel::SW;

    atmosphere.set_io(io);
    atmosphere.set_beta(beta);
    atmosphere.set_transmittance_model(transmittance_model);

    EXPECT_DOUBLE_EQ(atmosphere.io(), io);
    EXPECT_DOUBLE_EQ(atmosphere.beta(), beta);
    EXPECT_EQ(atmosphere.transmittance_model(), transmittance_model);
}

TEST(AtmosphereTest, TransmittanceVB)
{
    hypl::Atmosphere atmosphere(1100.0, 0.12, hypl::Atmosphere::TransmittanceModel::VB);

    ASSERT_EQ(atmosphere.transmittance_model(), hypl::Atmosphere::TransmittanceModel::VB);

    double minimum_slant_range = 50;
    double delta_distance = 53.2;
    double expected_transmittance;

    double slant_range = minimum_slant_range;
    while( slant_range < 1500.0)
    {
        expected_transmittance = 0.99326 + slant_range * (-0.0001046 + (1.7000000000000003e-8 - 2.845e-12 * slant_range) * slant_range);
        EXPECT_DOUBLE_EQ(atmosphere.Transmittance(slant_range), expected_transmittance );
        slant_range += delta_distance;
    }
}

TEST(AtmosphereTest, TransmittanceSW)
{
    double beta = 0.115;
    hypl::Atmosphere atmosphere(1150.0, beta, hypl::Atmosphere::TransmittanceModel::SW);

    ASSERT_FLOAT_EQ(atmosphere.beta(), beta);
    ASSERT_EQ(atmosphere.transmittance_model(), hypl::Atmosphere::TransmittanceModel::SW);

    double minimum_slant_range = 50;
    double delta_distance = 53.2;
    double expected_transmittance;

    double slant_range = minimum_slant_range;
    while( slant_range < 1500.0)
    {
        expected_transmittance = exp(-(0.000010696 + 0.0009196 * beta) * slant_range);
        EXPECT_DOUBLE_EQ(atmosphere.Transmittance(slant_range), expected_transmittance );
        slant_range += delta_distance;
    }
}

TEST(AtmosphereTest, TransmittanceLH)
{
    double beta = 0.105;
    hypl::Atmosphere atmosphere(1130.0, beta, hypl::Atmosphere::TransmittanceModel::LH);

    ASSERT_FLOAT_EQ(atmosphere.beta(), beta);
    ASSERT_EQ(atmosphere.transmittance_model(), hypl::Atmosphere::TransmittanceModel::LH);

    double minimum_slant_range = 50;
    double delta_distance = 53.2;
    double expected_transmittance;

    double slant_range = minimum_slant_range;
    while( slant_range < 1500.0)
    {
        if(slant_range <= 1000.0) expected_transmittance = 0.99321 + (-0.0001176 + 1.9699999999999998e-8 * slant_range) * slant_range;
        else expected_transmittance = exp(-0.0001106 * slant_range);
        EXPECT_DOUBLE_EQ(atmosphere.Transmittance(slant_range), expected_transmittance );
        slant_range += delta_distance;
    }
}

TEST(AtmosphereTest, DniFromSz)
{
    double io = 1130.0;
    double beta = 0.105;
    hypl::Atmosphere atmosphere(io, beta, hypl::Atmosphere::TransmittanceModel::LH);

    ASSERT_FLOAT_EQ(atmosphere.io(), io);
    ASSERT_FLOAT_EQ(atmosphere.beta(), beta);
    ASSERT_EQ(atmosphere.transmittance_model(), hypl::Atmosphere::TransmittanceModel::LH);

    double minimum_elevation = 0.0 * hypl::mathconstants::degree;
    double delta_elevation = 3.4 * hypl::mathconstants::degree;
    double expected_dni;

    double elevation = minimum_elevation;
    double sz = sin(elevation);
    while( elevation <= 90.0 * hypl::mathconstants::degree)
    {
        if(sz>0.00001) expected_dni = io * exp(-beta/sz);
        else expected_dni = 0.0;
        EXPECT_DOUBLE_EQ(atmosphere.DniFromSz(sz), expected_dni);
        elevation += delta_elevation;
        sz = sin(elevation);
    }
}

TEST(LocationTest, DefaultConstructorAndAssesors)
{
    hypl::Location location;
    double expected_latitude =  37.2 * hypl::mathconstants::degree;
    EXPECT_DOUBLE_EQ(location.latitude(), expected_latitude);
}

TEST(LocationTest, Constructor)
{
    double latitude = 40.0 * hypl::mathconstants::degree;
    hypl::Location location(latitude);
    double expected_latitude =  latitude;
    EXPECT_DOUBLE_EQ(location.latitude(), expected_latitude);
}

TEST(LocationTest, Mutators)
{
    double latitude = 50.0 * hypl::mathconstants::degree;
    hypl::Location location(latitude);
    double expected_latitude =  latitude;
    EXPECT_DOUBLE_EQ(location.latitude(), expected_latitude);
    
    double latitude_degree = -20.0;
    location.set_latitude_degree(latitude_degree);
    expected_latitude = latitude_degree * hypl::mathconstants::degree;
    EXPECT_DOUBLE_EQ(location.latitude(), expected_latitude);
}

TEST(LocationTest, LatitudeDegree)
{
    hypl::Location locationA;
    double latitude_degree = -31.3;
    locationA.set_latitude_degree(latitude_degree);
    EXPECT_DOUBLE_EQ(locationA.LatitudeDegree(), latitude_degree);

    latitude_degree = 46.98;
    double latitude = latitude_degree *  hypl::mathconstants::degree;
    hypl::Location locationB(latitude);    
    EXPECT_DOUBLE_EQ(locationB.LatitudeDegree(), latitude_degree);
}

TEST(LocationTest, HourAngleLimit)
{
    double latitude = -18.3 * hypl::mathconstants::degree;
    hypl::Location location(latitude);

    double minimum_declination = -23.45 * hypl::mathconstants::degree;
    double maximum_declination = -minimum_declination;
    double delta_declination = 0.56 * hypl::mathconstants::degree;
    double declination = minimum_declination;

    while( declination <= maximum_declination)
    {
        double computed_hour_angle_limit = location.HourAngleLimit(declination);
        double expected_hour_angle_limit = abs(acos(-tan(declination)*tan(latitude)));
        EXPECT_DOUBLE_EQ( computed_hour_angle_limit, 
                         expected_hour_angle_limit);
        declination += delta_declination;
    }
}

TEST(LocationTest, SolarVector)
{
    double latitude = 23.67 * hypl::mathconstants::degree;
    hypl::Location location(latitude);

    double cos_latitude = cos(latitude);
    double sin_latitude = sin(latitude);

    double minimum_declination = -23.45 * hypl::mathconstants::degree;
    double maximum_declination = -minimum_declination;
    double delta_declination = 0.56 * hypl::mathconstants::degree;
    double declination = minimum_declination;

    while( declination <= maximum_declination)
    {
        double cos_declination = cos(declination);
        double sin_declination = sin(declination);        
        
        double maximum_hour_angle = location.HourAngleLimit(declination);
        double minimum_hour_angle = -maximum_hour_angle;
        double delta_hour_angle = 1.5 * hypl::mathconstants::degree;
        double hour_angle = minimum_hour_angle;

        while( hour_angle <= maximum_hour_angle)
        {
            double cos_hour_angle = cos(hour_angle);
            hypl::vec3d expected_solar_vector = hypl::vec3d(
                            -cos_declination * sin(hour_angle),
                            sin_declination * cos_latitude - cos_declination * sin_latitude * cos_hour_angle,
                            sin_declination * sin_latitude + cos_declination * cos_latitude * cos_hour_angle );

            hypl::vec3d computed_solar_vector = location.SolarVector(hour_angle, declination);

            EXPECT_DOUBLE_EQ(computed_solar_vector.x, expected_solar_vector.x);
            EXPECT_DOUBLE_EQ(computed_solar_vector.x, expected_solar_vector.x);
            EXPECT_DOUBLE_EQ(computed_solar_vector.x, expected_solar_vector.x);
            hour_angle += delta_hour_angle;
        }
        declination += delta_declination;
    }
}

/*
        Boundaries(double xmin, double xmax, double ymin, double ymax)
            : m_xmin {xmin},
              m_xmax {xmax},
              m_ymin {ymin},
              m_ymax {ymax} {}
*/


TEST(BoundariesTest, DefaultConstructor)
{
    hypl::Boundaries boundaries;
    EXPECT_DOUBLE_EQ(boundaries.xmin(), -500.0);
    EXPECT_DOUBLE_EQ(boundaries.xmax(), 500.0);
    EXPECT_DOUBLE_EQ(boundaries.ymin(), -100.0);
    EXPECT_DOUBLE_EQ(boundaries.ymax(),  700.0);
}

TEST(BoundariesTest, Constructor)
{
    double xmin = 300.0;
    double xmax = 567.0;
    double ymin = -300.0;
    double ymax = 3000.0;

    hypl::Boundaries boundaries(xmin, xmax, ymin, ymax);
    EXPECT_DOUBLE_EQ(boundaries.xmin(), xmin);
    EXPECT_DOUBLE_EQ(boundaries.xmax(), xmax);
    EXPECT_DOUBLE_EQ(boundaries.ymin(), ymin);
    EXPECT_DOUBLE_EQ(boundaries.ymax(), ymax);
}

TEST(BoundariesTest, MutatorsAndAccessors)
{
    hypl::Boundaries boundaries;
    EXPECT_DOUBLE_EQ(boundaries.xmin(), -500.0);
    EXPECT_DOUBLE_EQ(boundaries.xmax(), 500.0);
    EXPECT_DOUBLE_EQ(boundaries.ymin(), -100.0);
    EXPECT_DOUBLE_EQ(boundaries.ymax(),  700.0);

    double xmin = -312.0;
    double xmax = 800.0;
    double ymin = -300.0;
    double ymax = 1600.0;

    boundaries.set_xmin(xmin);
    boundaries.set_xmax(xmax);
    boundaries.set_ymin(ymin);
    boundaries.set_ymax(ymax);

    EXPECT_DOUBLE_EQ(boundaries.xmin(), xmin);
    EXPECT_DOUBLE_EQ(boundaries.xmax(), xmax);
    EXPECT_DOUBLE_EQ(boundaries.ymin(), ymin);
    EXPECT_DOUBLE_EQ(boundaries.ymax(), ymax);
}

TEST(EnvironmentTest, DefaultConstructorAndAccessors)
{
    hypl::Environment environment;

    const hypl::Location& location = environment.location();
    const hypl::Atmosphere& atmosphere = environment.atmosphere();
    
    double computed_latitude = location.latitude();
    double computed_io = atmosphere.io();
    double computed_beta = atmosphere.beta();
    hypl::Atmosphere::TransmittanceModel computed_transmittance_model = atmosphere.transmittance_model();

    double expected_latitude = 37.2 * hypl::mathconstants::degree;
    double expected_io = 1110.0;
    double expected_beta = 0.11;
    hypl::Atmosphere::TransmittanceModel expected_transmittance_model = hypl::Atmosphere::TransmittanceModel::VB;

    EXPECT_DOUBLE_EQ(computed_latitude, expected_latitude);
    EXPECT_DOUBLE_EQ(computed_io, expected_io);
    EXPECT_DOUBLE_EQ(computed_beta, expected_beta);
    EXPECT_EQ(computed_transmittance_model, expected_transmittance_model);
}

TEST(EnvironmentTest, Constructor)
{
    double expected_latitude = -7.2 * hypl::mathconstants::degree;
    hypl::Location expected_location(expected_latitude);

    double expected_io = 1030.4;
    double expected_beta = 0.1093;
    hypl::Atmosphere::TransmittanceModel expected_transmittance_model = hypl::Atmosphere::TransmittanceModel::SW;
    hypl::Atmosphere expected_atmosphere(expected_io, expected_beta, expected_transmittance_model);

    hypl::Environment environment(expected_location, expected_atmosphere);


    const hypl::Location& computed_location = environment.location();
    const hypl::Atmosphere& computed_atmosphere = environment.atmosphere();

    double computed_latitude = computed_location.latitude();
    double computed_io = computed_atmosphere.io();
    double computed_beta = computed_atmosphere.beta();
    hypl::Atmosphere::TransmittanceModel computed_transmittance_model = computed_atmosphere.transmittance_model();

    EXPECT_DOUBLE_EQ(computed_latitude, expected_latitude);
    EXPECT_DOUBLE_EQ(computed_io, expected_io);
    EXPECT_DOUBLE_EQ(computed_beta, expected_beta);
    EXPECT_EQ(computed_transmittance_model, expected_transmittance_model);
}

TEST(EnvironmentTest, Mutators)
{
    // Setting up the initial values and creating the environment
    double initial_latitude = 26.45 * hypl::mathconstants::degree;
    hypl::Location initial_location(initial_latitude);

    double initial_io = 1123.6;
    double initial_beta = 0.1902;
    hypl::Atmosphere::TransmittanceModel initial_transmittance_model = hypl::Atmosphere::TransmittanceModel::LH;
    hypl::Atmosphere initial_atmosphere(initial_io, initial_beta, initial_transmittance_model);

    hypl::Environment environment(initial_location, initial_atmosphere);


    const hypl::Location& computed_location = environment.location();
    const hypl::Atmosphere& computed_atmosphere = environment.atmosphere();

    double computed_latitude = computed_location.latitude();
    double computed_io = computed_atmosphere.io();
    double computed_beta = computed_atmosphere.beta();
    hypl::Atmosphere::TransmittanceModel computed_transmittance_model = computed_atmosphere.transmittance_model();

    EXPECT_DOUBLE_EQ(computed_latitude, initial_latitude);
    EXPECT_DOUBLE_EQ(computed_io, initial_io);
    EXPECT_DOUBLE_EQ(computed_beta, initial_beta);
    EXPECT_EQ(computed_transmittance_model, initial_transmittance_model);
/*
        void set_location(Location location);
        void set_atmosphere(Atmosphere atmosphere);
        void set_delta_days(int delta_days);
        void set_location_and_atmosphere(Location location, Atmosphere atmosphere);
*/

    // Changing the content of the environment with mutators
    double new_latitude = -50.31 * hypl::mathconstants::degree;
    hypl::Location new_location(new_latitude);
    environment.set_location(new_location);

    double new_io = 999.466;
    double new_beta = 0.122;
    hypl::Atmosphere::TransmittanceModel new_transmittance_model = hypl::Atmosphere::TransmittanceModel::VB;
    hypl::Atmosphere new_atmosphere(new_io, new_beta, new_transmittance_model);
    environment.set_atmosphere(new_atmosphere);


    const hypl::Location& new_computed_location = environment.location();
    const hypl::Atmosphere& new_computed_atmosphere = environment.atmosphere();

    double new_computed_latitude = new_computed_location.latitude();
    double new_computed_io = new_computed_atmosphere.io();
    double new_computed_beta = new_computed_atmosphere.beta();
    hypl::Atmosphere::TransmittanceModel new_computed_transmittance_model = new_computed_atmosphere.transmittance_model();

    EXPECT_DOUBLE_EQ(new_computed_latitude, new_latitude);
    EXPECT_DOUBLE_EQ(new_computed_io, new_io);
    EXPECT_DOUBLE_EQ(new_computed_beta, new_beta);
    EXPECT_EQ(new_computed_transmittance_model, new_transmittance_model);

    //Testing the last mutator
    environment.set_location_and_atmosphere(initial_location, initial_atmosphere);


    const hypl::Location& new_computed_location2 = environment.location();
    const hypl::Atmosphere& new_computed_atmosphere2 = environment.atmosphere();

    double new_computed_latitude2 = new_computed_location2.latitude();
    double new_computed_io2 = new_computed_atmosphere2.io();
    double new_computed_beta2 = new_computed_atmosphere2.beta();
    hypl::Atmosphere::TransmittanceModel new_computed_transmittance_model2 = new_computed_atmosphere2.transmittance_model();

    EXPECT_DOUBLE_EQ(new_computed_latitude2, initial_latitude);
    EXPECT_DOUBLE_EQ(new_computed_io2, initial_io);
    EXPECT_DOUBLE_EQ(new_computed_beta2, initial_beta);
    EXPECT_EQ(new_computed_transmittance_model2, initial_transmittance_model);
}

class SimpleFunctor
{
public:
    SimpleFunctor(double (&function)(double)) : m_function {function} {}
    double operator()(double x) const { return m_function(x); }

private:
double (&m_function)(double);
};




TEST(AuxfunctionTest, SolarDeclinationByDayNumber)
{
    int minimum_day_number = 1;
    int delta_day_number = 4;
    int day_number = minimum_day_number;
    double expected_solar_declination_by_day_number;

    const double k = (2. * hypl::mathconstants::pi)/365.;
    while( day_number < 365)
    {
        double omega = k * (day_number - 1.0);
        expected_solar_declination_by_day_number= 0.006918 - 0.399912 * cos(omega) + 0.070257 * sin(omega)
                                  - 0.006758 * cos(2.*omega) + 0.000907 * sin(2.*omega)
                                  - 0.002697 * cos(3.*omega) + 0.001480 * sin(3.*omega);
        double computed_declination = hypl::auxfunction::SolarDeclinationByDayNumber(day_number);
        EXPECT_DOUBLE_EQ( computed_declination, 
                         expected_solar_declination_by_day_number) << "day number = " << day_number << std::endl;
        day_number += delta_day_number;
    }
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}