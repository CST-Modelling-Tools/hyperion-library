#include <gtest/gtest.h>
#include <math.h>
#include "mathconstants.h"
#include "atmosphere.h"
#include "location.h"
#include "boundaries.h"
#include "environment.h"
#include "deintegrator.h"
#include "dniashrae.h"
#include "auxfunction.h"

TEST(AtmosphereTest, DefaultConstructor)
{
    hypl::Atmosphere atmosphere;

    EXPECT_DOUBLE_EQ(atmosphere.io(), 1110.0);
    EXPECT_DOUBLE_EQ(atmosphere.beta(), 0.11);
    EXPECT_EQ(atmosphere.attenuation_model(), hypl::Atmosphere::AttenuationModel::VB);
}

TEST(AtmosphereTest, Constructor)
{
    double io = 1120.0;
    double beta = 0.10;
    hypl::Atmosphere::AttenuationModel attenuation_model = hypl::Atmosphere::AttenuationModel::LH;

    hypl::Atmosphere atmosphere(io, beta, attenuation_model);

    EXPECT_DOUBLE_EQ(atmosphere.io(), io);
    EXPECT_DOUBLE_EQ(atmosphere.beta(), beta);
    EXPECT_EQ(atmosphere.attenuation_model(), attenuation_model);
}

TEST(AtmosphereTest, MutatorsAndAccessors)
{
    hypl::Atmosphere atmosphere;
    double io = 1110.0;
    double beta = 0.11;
    hypl::Atmosphere::AttenuationModel attenuation_model = hypl::Atmosphere::AttenuationModel::VB;

    EXPECT_DOUBLE_EQ(atmosphere.io(), io);
    EXPECT_DOUBLE_EQ(atmosphere.beta(), beta);
    EXPECT_EQ(atmosphere.attenuation_model(), attenuation_model);

    io = 2120.0;
    beta = 0.20;
    attenuation_model = hypl::Atmosphere::AttenuationModel::SW;

    atmosphere.set_io(io);
    atmosphere.set_beta(beta);
    atmosphere.set_attenuation_model(attenuation_model);

    EXPECT_DOUBLE_EQ(atmosphere.io(), io);
    EXPECT_DOUBLE_EQ(atmosphere.beta(), beta);
    EXPECT_EQ(atmosphere.attenuation_model(), attenuation_model);
}

TEST(AtmosphereTest, AttenuationVB)
{
    hypl::Atmosphere atmosphere(1100.0, 0.12, hypl::Atmosphere::AttenuationModel::VB);

    ASSERT_EQ(atmosphere.attenuation_model(), hypl::Atmosphere::AttenuationModel::VB);

    double minimum_slant_range = 50;
    double delta_distance = 53.2;
    double expected_attenuation;

    double slant_range = minimum_slant_range;
    while( slant_range < 1500.0)
    {
        expected_attenuation = 0.99326 + slant_range * (-0.0001046 + (1.7000000000000003e-8 - 2.845e-12 * slant_range) * slant_range);
        EXPECT_DOUBLE_EQ(atmosphere.Attenuation(slant_range), expected_attenuation );
        slant_range += delta_distance;
    }
}

TEST(AtmosphereTest, AttenuationSW)
{
    double beta = 0.115;
    hypl::Atmosphere atmosphere(1150.0, beta, hypl::Atmosphere::AttenuationModel::SW);

    ASSERT_FLOAT_EQ(atmosphere.beta(), beta);
    ASSERT_EQ(atmosphere.attenuation_model(), hypl::Atmosphere::AttenuationModel::SW);

    double minimum_slant_range = 50;
    double delta_distance = 53.2;
    double expected_attenuation;

    double slant_range = minimum_slant_range;
    while( slant_range < 1500.0)
    {
        expected_attenuation = exp(-(0.000010696 + 0.0009196 * beta) * slant_range);
        EXPECT_DOUBLE_EQ(atmosphere.Attenuation(slant_range), expected_attenuation );
        slant_range += delta_distance;
    }
}

TEST(AtmosphereTest, AttenuationLH)
{
    double beta = 0.105;
    hypl::Atmosphere atmosphere(1130.0, beta, hypl::Atmosphere::AttenuationModel::LH);

    ASSERT_FLOAT_EQ(atmosphere.beta(), beta);
    ASSERT_EQ(atmosphere.attenuation_model(), hypl::Atmosphere::AttenuationModel::LH);

    double minimum_slant_range = 50;
    double delta_distance = 53.2;
    double expected_attenuation;

    double slant_range = minimum_slant_range;
    while( slant_range < 1500.0)
    {
        if(slant_range <= 1000.0) expected_attenuation = 0.99321 + (-0.0001176 + 1.9699999999999998e-8 * slant_range) * slant_range;
        else expected_attenuation = exp(-0.0001106 * slant_range);
        EXPECT_DOUBLE_EQ(atmosphere.Attenuation(slant_range), expected_attenuation );
        slant_range += delta_distance;
    }
}

TEST(AtmosphereTest, DniFromSz)
{
    double io = 1130.0;
    double beta = 0.105;
    hypl::Atmosphere atmosphere(io, beta, hypl::Atmosphere::AttenuationModel::LH);

    ASSERT_FLOAT_EQ(atmosphere.io(), io);
    ASSERT_FLOAT_EQ(atmosphere.beta(), beta);
    ASSERT_EQ(atmosphere.attenuation_model(), hypl::Atmosphere::AttenuationModel::LH);

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

    double computed_dni_annual_energy = environment.dni_annual_energy();

    const hypl::Location& location = environment.location();
    const hypl::Atmosphere& atmosphere = environment.atmosphere();
    
    double computed_latitude = location.latitude();
    double computed_io = atmosphere.io();
    double computed_beta = atmosphere.beta();
    hypl::Atmosphere::AttenuationModel computed_attenuation_model = atmosphere.attenuation_model();

    double expected_latitude = 37.2 * hypl::mathconstants::degree;
    double expected_io = 1110.0;
    double expected_beta = 0.11;
    hypl::Atmosphere::AttenuationModel expected_attenuation_model = hypl::Atmosphere::AttenuationModel::VB;
    int expected_delta_days = 13;
    double expected_dni_annual_energy = hypl::auxfunction::DniYearlyEnergy(environment);

    EXPECT_DOUBLE_EQ(computed_latitude, expected_latitude);
    EXPECT_DOUBLE_EQ(computed_io, expected_io);
    EXPECT_DOUBLE_EQ(computed_beta, expected_beta);
    EXPECT_EQ(computed_attenuation_model, expected_attenuation_model);
    EXPECT_DOUBLE_EQ(computed_dni_annual_energy, expected_dni_annual_energy);    
}

TEST(EnvironmentTest, Constructor)
{
    double expected_latitude = -7.2 * hypl::mathconstants::degree;
    hypl::Location expected_location(expected_latitude);

    double expected_io = 1030.4;
    double expected_beta = 0.1093;
    hypl::Atmosphere::AttenuationModel expected_attenuation_model = hypl::Atmosphere::AttenuationModel::SW;
    hypl::Atmosphere expected_atmosphere(expected_io, expected_beta, expected_attenuation_model);

    hypl::Environment environment(expected_location, expected_atmosphere);

    double expected_dni_annual_energy = hypl::auxfunction::DniYearlyEnergy(environment);

    double computed_dni_annual_energy = environment.dni_annual_energy();

    const hypl::Location& computed_location = environment.location();
    const hypl::Atmosphere& computed_atmosphere = environment.atmosphere();

    double computed_latitude = computed_location.latitude();
    double computed_io = computed_atmosphere.io();
    double computed_beta = computed_atmosphere.beta();
    hypl::Atmosphere::AttenuationModel computed_attenuation_model = computed_atmosphere.attenuation_model();

    EXPECT_DOUBLE_EQ(computed_latitude, expected_latitude);
    EXPECT_DOUBLE_EQ(computed_io, expected_io);
    EXPECT_DOUBLE_EQ(computed_beta, expected_beta);
    EXPECT_EQ(computed_attenuation_model, expected_attenuation_model);
    EXPECT_DOUBLE_EQ(computed_dni_annual_energy, expected_dni_annual_energy);
}

TEST(EnvironmentTest, Mutators)
{
    // Setting up the initial values and creating the environment
    double initial_latitude = 26.45 * hypl::mathconstants::degree;
    hypl::Location initial_location(initial_latitude);

    double initial_io = 1123.6;
    double initial_beta = 0.1902;
    hypl::Atmosphere::AttenuationModel initial_attenuation_model = hypl::Atmosphere::AttenuationModel::LH;
    hypl::Atmosphere initial_atmosphere(initial_io, initial_beta, initial_attenuation_model);

    hypl::Environment environment(initial_location, initial_atmosphere);

    // Verifying that the content of the environment is consistent with the initial values
    double initial_dni_annual_energy = hypl::auxfunction::DniYearlyEnergy(environment);

    double computed_dni_annual_energy = environment.dni_annual_energy();

    const hypl::Location& computed_location = environment.location();
    const hypl::Atmosphere& computed_atmosphere = environment.atmosphere();

    double computed_latitude = computed_location.latitude();
    double computed_io = computed_atmosphere.io();
    double computed_beta = computed_atmosphere.beta();
    hypl::Atmosphere::AttenuationModel computed_attenuation_model = computed_atmosphere.attenuation_model();

    EXPECT_DOUBLE_EQ(computed_latitude, initial_latitude);
    EXPECT_DOUBLE_EQ(computed_io, initial_io);
    EXPECT_DOUBLE_EQ(computed_beta, initial_beta);
    EXPECT_EQ(computed_attenuation_model, initial_attenuation_model);
    EXPECT_DOUBLE_EQ(computed_dni_annual_energy, initial_dni_annual_energy);
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
    hypl::Atmosphere::AttenuationModel new_attenuation_model = hypl::Atmosphere::AttenuationModel::VB;
    hypl::Atmosphere new_atmosphere(new_io, new_beta, new_attenuation_model);
    environment.set_atmosphere(new_atmosphere);


    // Verifying that the content of the environment is consistent with the new values    
    double new_dni_annual_energy = hypl::auxfunction::DniYearlyEnergy(environment);

    double new_computed_dni_annual_energy = environment.dni_annual_energy();

    const hypl::Location& new_computed_location = environment.location();
    const hypl::Atmosphere& new_computed_atmosphere = environment.atmosphere();

    double new_computed_latitude = new_computed_location.latitude();
    double new_computed_io = new_computed_atmosphere.io();
    double new_computed_beta = new_computed_atmosphere.beta();
    hypl::Atmosphere::AttenuationModel new_computed_attenuation_model = new_computed_atmosphere.attenuation_model();

    EXPECT_DOUBLE_EQ(new_computed_latitude, new_latitude);
    EXPECT_DOUBLE_EQ(new_computed_io, new_io);
    EXPECT_DOUBLE_EQ(new_computed_beta, new_beta);
    EXPECT_EQ(new_computed_attenuation_model, new_attenuation_model);
    EXPECT_DOUBLE_EQ(new_computed_dni_annual_energy, new_dni_annual_energy);

    //Testing the last mutator
    environment.set_location_and_atmosphere(initial_location, initial_atmosphere);

    // Verifying that the content of the environment is consistent with the new values    
    double new_dni_annual_energy2 = hypl::auxfunction::DniYearlyEnergy(environment);

    double new_computed_dni_annual_energy2 = environment.dni_annual_energy();

    const hypl::Location& new_computed_location2 = environment.location();
    const hypl::Atmosphere& new_computed_atmosphere2 = environment.atmosphere();

    double new_computed_latitude2 = new_computed_location2.latitude();
    double new_computed_io2 = new_computed_atmosphere2.io();
    double new_computed_beta2 = new_computed_atmosphere2.beta();
    hypl::Atmosphere::AttenuationModel new_computed_attenuation_model2 = new_computed_atmosphere2.attenuation_model();

    EXPECT_DOUBLE_EQ(new_computed_latitude2, initial_latitude);
    EXPECT_DOUBLE_EQ(new_computed_io2, initial_io);
    EXPECT_DOUBLE_EQ(new_computed_beta2, initial_beta);
    EXPECT_EQ(new_computed_attenuation_model2, initial_attenuation_model);
    EXPECT_DOUBLE_EQ(new_computed_dni_annual_energy2, new_dni_annual_energy2);
}

class SimpleFunctor
{
public:
    SimpleFunctor(double (&function)(double)) : m_function {function} {}
    double operator()(double x) const { return m_function(x); }

private:
double (&m_function)(double);
};

TEST(DEIntegratorTest, Integrate)
{
    // Integration of cosine
    SimpleFunctor CosineFunctor(cos);

    double a = 0.0;
    double b = hypl::mathconstants::pi;

    double computed_value = hypl::DEIntegrator<SimpleFunctor>::Integrate(CosineFunctor, a, b, 1e-6);
    double expected_value = 0.0;
    EXPECT_NEAR(computed_value, expected_value, 1e-13);

    b = hypl::mathconstants::pi/3.0;  
    computed_value = hypl::DEIntegrator<SimpleFunctor>::Integrate(CosineFunctor, a, b, 1e-6);
    expected_value = 0.8660254037844386;
    EXPECT_NEAR(computed_value, expected_value, 1e-13);

    b = hypl::mathconstants::pi/4.0;
    computed_value = hypl::DEIntegrator<SimpleFunctor>::Integrate(CosineFunctor, a, b, 1e-6);
    expected_value = 0.7071067811865476;
    EXPECT_NEAR(computed_value, expected_value, 1e-13);

    b = hypl::mathconstants::pi/5.0;
    computed_value = hypl::DEIntegrator<SimpleFunctor>::Integrate(CosineFunctor, a, b, 1e-6);
    expected_value = 0.5877852522924731;
    EXPECT_NEAR(computed_value, expected_value, 1e-13);

    // Integration of tangent
    SimpleFunctor TanFunctor(tan);
    
    a = 0.0;
    b = hypl::mathconstants::pi/2.2;

    computed_value = hypl::DEIntegrator<SimpleFunctor>::Integrate(TanFunctor, a, b, 1e-6);
    expected_value = 1.9497135047385736;
    EXPECT_NEAR(computed_value, expected_value, 1e-13);

    b = hypl::mathconstants::pi/3.0;  
    computed_value = hypl::DEIntegrator<SimpleFunctor>::Integrate(TanFunctor, a, b, 1e-6);
    expected_value = 0.6931471805599271;
    EXPECT_NEAR(computed_value, expected_value, 1e-13);

    b = hypl::mathconstants::pi/4.0;
    computed_value = hypl::DEIntegrator<SimpleFunctor>::Integrate(TanFunctor, a, b, 1e-6);
    expected_value = 0.34657359027996476;
    EXPECT_NEAR(computed_value, expected_value, 1e-13);

    b = hypl::mathconstants::pi/5.0;
    computed_value = hypl::DEIntegrator<SimpleFunctor>::Integrate(TanFunctor, a, b, 1e-6);
    expected_value = 0.2119353555003373;
    EXPECT_NEAR(computed_value, expected_value, 1e-13);

    // Integration of exponential
    SimpleFunctor ExpFunctor(exp);
    
    a = -10.0;
    b = hypl::mathconstants::pi/2.0;

    computed_value = hypl::DEIntegrator<SimpleFunctor>::Integrate(ExpFunctor, a, b, 1e-6);
    expected_value = 4.810431981035513;
    EXPECT_NEAR(computed_value, expected_value, 1e-12);

    b = hypl::mathconstants::pi/3.0;  
    computed_value = hypl::DEIntegrator<SimpleFunctor>::Integrate(ExpFunctor, a, b, 1e-6);
    expected_value = 2.849608508296569;
    EXPECT_NEAR(computed_value, expected_value, 1e-12);

    b = hypl::mathconstants::pi/4.0;
    computed_value = hypl::DEIntegrator<SimpleFunctor>::Integrate(ExpFunctor, a, b, 1e-6);
    expected_value = 2.193234650808236;
    EXPECT_NEAR(computed_value, expected_value, 1e-12);

    b = hypl::mathconstants::pi/5.0;
    computed_value = hypl::DEIntegrator<SimpleFunctor>::Integrate(ExpFunctor, a, b, 1e-6);
    expected_value = 1.874410687655564;
    EXPECT_NEAR(computed_value, expected_value, 1e-12);
}

TEST(DniASHRAETest, ConstructorANDAssesors)
{
    double latitude = 20.3 * hypl::mathconstants::degree; 
    hypl::Location location(latitude);

    double io = 1121.2;
    double beta = 0.158;
    hypl::Atmosphere::AttenuationModel attenuation_model = hypl::Atmosphere::AttenuationModel::VB;
    hypl::Atmosphere atmosphere(io, beta, attenuation_model);

    int delta_days = 13;
    hypl::Environment environment(location, atmosphere);

    double x = 0.3;
    double declination = 23.45 * (2.0 * x - 1.0) * hypl::mathconstants::degree;
    hypl::DniASHRAE dniASHRAE(declination, environment);

    double computed_declination = dniASHRAE.declination();
    ASSERT_DOUBLE_EQ(computed_declination, declination);

    const hypl::Environment& computed_environment = dniASHRAE.environment();
    const hypl::Location& computed_location = computed_environment.location();
    const hypl::Atmosphere& computed_atmosphere = computed_environment.atmosphere();

    double computed_latitude = computed_location.latitude();
    double computed_io = computed_atmosphere.io();
    double computed_beta = computed_atmosphere.beta();

    ASSERT_DOUBLE_EQ(computed_latitude, latitude);
    ASSERT_DOUBLE_EQ(computed_io, io);    
    ASSERT_DOUBLE_EQ(computed_beta, beta);
}

TEST(DniASHRAETest, operatorFunction)
{
    double latitude = 18.23 * hypl::mathconstants::degree; 
    hypl::Location location(latitude);

    double io = 1000.93;
    double beta = 0.106;
    hypl::Atmosphere::AttenuationModel attenuation_model = hypl::Atmosphere::AttenuationModel::LH;
    hypl::Atmosphere atmosphere(io, beta, attenuation_model);

    hypl::Environment environment(location, atmosphere);

    double x = 0.67;
    double declination = 23.45 * (2.0 * x - 1.0) * hypl::mathconstants::degree;
    hypl::DniASHRAE dniASHRAE(declination, environment);

    double computed_declination = dniASHRAE.declination();
    ASSERT_DOUBLE_EQ(computed_declination, declination);

    const hypl::Environment& computed_environment = dniASHRAE.environment();
    const hypl::Location& computed_location = computed_environment.location();
    const hypl::Atmosphere& computed_atmosphere = computed_environment.atmosphere();

    double computed_latitude = computed_location.latitude();
    double computed_io = computed_atmosphere.io();
    double computed_beta = computed_atmosphere.beta();

    ASSERT_DOUBLE_EQ(computed_latitude, latitude);
    ASSERT_DOUBLE_EQ(computed_io, io);    
    ASSERT_DOUBLE_EQ(computed_beta, beta);


    double hour_angle_limit = location.HourAngleLimit(declination);
    double solar_time_start = -hour_angle_limit/hypl::mathconstants::earth_rotational_speed; // start time in seconds
    double solar_time_finish = - solar_time_start; // solar time in seconds, zero at solar noon, negative before and positive after.
    double delta_solar_time = 78.5; // seconds
    double solar_time = solar_time_start;

    while( solar_time < solar_time_finish)
    {
        double hour_angle = solar_time * hypl::mathconstants::earth_rotational_speed;
        double sz = sin(declination) * sin(latitude) + cos(declination) * cos(latitude) * cos(hour_angle);
        
        double expected_dni;
        if(sz>0.00001) expected_dni = io * exp(-beta/sz);
        else expected_dni = 0.0;

        double computed_dni = dniASHRAE(solar_time);

        ASSERT_DOUBLE_EQ(computed_dni, expected_dni);
        solar_time += delta_solar_time;
    }
}

TEST(AuxfunctionTest, DayNumber)
{
    int minimum_day_index = 1;
    int delta_day_index = 7;
    int day_index = minimum_day_index;
    int expected_day_number;
    while( day_index < 365)
    {
        expected_day_number = (day_index + 355) % 365;
        EXPECT_EQ( hypl::auxfunction::DayNumber(day_index), expected_day_number);
        day_index += delta_day_index;
    }
}

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

TEST(AuxfunctionTest, SolarDeclinationByIndex)
{
    int minimum_day_index = 1;
    int delta_day_index = 5;
    int day_index = minimum_day_index;
    double expected_solar_declination_by_day_index;

    const double k = (2. * hypl::mathconstants::pi)/365.;
    while( day_index < 365)
    {
        double day_number = (day_index + 355) % 365;
        double omega = k * (day_number - 1.0);
        expected_solar_declination_by_day_index = 0.006918 - 0.399912 * cos(omega) + 0.070257 * sin(omega)
                                  - 0.006758 * cos(2.*omega) + 0.000907 * sin(2.*omega)
                                  - 0.002697 * cos(3.*omega) + 0.001480 * sin(3.*omega);
        EXPECT_DOUBLE_EQ( hypl::auxfunction::SolarDeclinationByIndex(day_index), 
                         expected_solar_declination_by_day_index) << "day number = " << day_number << std::endl;
        day_index += delta_day_index;
    }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}