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
    hypl::Environment environment(location, atmosphere, delta_days);

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

    int delta_days = 13;
    hypl::Environment environment(location, atmosphere, delta_days);

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

TEST(AuxfunctionTest, DniDailyEnergy)
{
    const int nrows = 36;
    const int ncolumns = 5;
    double expected_values[nrows][nrows] = { 
        {-0.3303908274025265, -0.494626310015193, 940.8, 0.2345, 2.752001284536965e7},
        {-0.3303908274025265, -0.494626310015193, 940.8, 0.101, 3.515025596715044e7},
        {-0.3303908274025265, -0.494626310015193, 1201.46, 0.2345, 3.514476470365415e7},
        {-0.3303908274025265, -0.494626310015193, 1201.46, 0.101, 4.488905881621234e7},
        {-0.3303908274025265, 0.002443460952792062, 940.8, 0.2345, 2.442451219326312e7},
        {-0.3303908274025265, 0.002443460952792062, 940.8, 0.101, 3.134837121108109e7},
        {-0.3303908274025265, 0.002443460952792062, 1201.46, 0.2345, 3.119161821823757e7},
        {-0.3303908274025265, 0.002443460952792062, 1201.46, 0.101, 4.003381598136214e7},
        {-0.3303908274025265, 0.991347015132779, 940.8, 0.2345, 6.082032643181693e6},
        {-0.3303908274025265, 0.991347015132779, 940.8, 0.101, 1.26584900533368e7},
        {-0.3303908274025265, 0.991347015132779, 1201.46, 0.2345, 7.767133226484989e6},
        {-0.3303908274025265, 0.991347015132779, 1201.46, 0.101, 1.616567757172835e7},
        {0.2478367537831948, -0.494626310015193, 940.8, 0.2345, 2.008532715097575e7},
        {0.2478367537831948, -0.494626310015193, 940.8, 0.101, 2.716703946630119e7},
        {0.2478367537831948, -0.494626310015193, 1201.46, 0.2345, 2.565020956506306e7},
        {0.2478367537831948, -0.494626310015193, 1201.46, 0.101, 3.469399578782125e7},
        {0.2478367537831948, 0.002443460952792062, 940.8, 0.2345, 2.470287945019904e7},
        {0.2478367537831948, 0.002443460952792062, 940.8, 0.101, 3.154467271148444e7},
        {0.2478367537831948, 0.002443460952792062, 1201.46, 0.2345, 3.154711048494489e7},
        {0.2478367537831948, 0.002443460952792062, 1201.46, 0.101, 4.028450518275948e7},
        {0.2478367537831948, 0.991347015132779, 940.8, 0.2345, 2.660085738338308e7},
        {0.2478367537831948, 0.991347015132779, 940.8, 0.101, 3.63938289834597e7},
        {0.2478367537831948, 0.991347015132779, 1201.46, 0.2345, 3.397094612227831e7},
        {0.2478367537831948, 0.991347015132779, 1201.46, 0.101, 4.647717875262276e7},
        {0.3490658503988659, -0.494626310015193, 940.8, 0.2345, 1.829669899718514e7},
        {0.3490658503988659, -0.494626310015193, 940.8, 0.101, 2.537682750305485e7},
        {0.3490658503988659, -0.494626310015193, 1201.46, 0.2345, 2.336602038388398e7},
        {0.3490658503988659, -0.494626310015193, 1201.46, 0.101, 3.240778398365252e7},
        {0.3490658503988659, 0.002443460952792062, 940.8, 0.2345, 2.440071696976267e7},
        {0.3490658503988659, 0.002443460952792062, 940.8, 0.101, 3.134916803744038e7},
        {0.3490658503988659, 0.002443460952792062, 1201.46, 0.2345, 3.116123024074304e7},
        {0.3490658503988659, 0.002443460952792062, 1201.46, 0.101, 4.003483357808579e7},
        {0.3490658503988659, 0.991347015132779, 940.8, 0.2345, 2.990615635957539e7},
        {0.3490658503988659, 0.991347015132779, 940.8, 0.101, 4.031944766765073e7},
        {0.3490658503988659, 0.991347015132779, 1201.46, 0.2345, 3.819201809074771e7},
        {0.3490658503988659, 0.991347015132779, 1201.46, 0.101, 5.149043749444693e7}      
    };

    int delta_days = 13;
    hypl::Atmosphere::AttenuationModel attenuation_model = hypl::Atmosphere::AttenuationModel::VB;

    for( int i = 0; i < nrows; i++)
    {
        double declination = expected_values[i][0];
        double latitude = expected_values[i][1];
        double io = expected_values[i][2];
        double beta = expected_values[i][3];
        double expected_dni_daily_energy = expected_values[i][4];

        hypl::Location location(latitude);
        hypl::Atmosphere atmosphere(io, beta, attenuation_model);
        hypl::Environment environment(location, atmosphere, delta_days);
        double computed_dni_daily_energy =  hypl::auxfunction::DniDailyEnergy(declination, environment);

        double percentage_error = 100 * ( (computed_dni_daily_energy/expected_dni_daily_energy) - 1.0 );
        EXPECT_NEAR(percentage_error, 0.0, 5e-6) << "i = " << i;
    }
}

TEST(AuxfunctionTest, DniYearlyEnergy)
{
    const int nrows = 27;
    const int ncolumns = 4;
    double expected_values[nrows][nrows] = { 
        {-0.7965682706102121, 1063.8, 0.2345, 8.46000843768372e9},
        {-0.7965682706102121, 1063.8, 0.101, 1.176027950784292e10},
        {-0.7965682706102121, 1063.8, 0.158, 1.013981107441368e10},
        {-0.7965682706102121, 879.64, 0.2345, 6.995451985452253e9},
        {-0.7965682706102121, 879.64, 0.101, 9.72439581338499e9},
        {-0.7965682706102121, 879.64, 0.158, 8.38445517343227e9},
        {-0.7965682706102121, 1101.27, 0.2345, 8.75799350645606e9},
        {-0.7965682706102121, 1101.27, 0.101, 1.217450931904698e10},
        {-0.7965682706102121, 1101.27, 0.158, 1.049696347238161e10},
        {0.2246238747316702, 1063.8, 0.2345, 1.005158683990919e10},
        {0.2246238747316702, 1063.8, 0.101, 1.292738237183389e10},
        {0.2246238747316702, 1063.8, 0.158, 1.154892767746369e10},
        {0.2246238747316702, 879.64, 0.2345, 8.31150389909542e9},
        {0.2246238747316702, 879.64, 0.101, 1.06894553765369e10},
        {0.2246238747316702, 879.64, 0.158, 9.54963220737371e9},
        {0.2246238747316702, 1101.27, 0.2345, 1.040563173452415e10},
        {0.2246238747316702, 1101.27, 0.101, 1.338272079773407e10},
        {0.2246238747316702, 1101.27, 0.158, 1.195571308832527e10},
        {0.6597344572538565, 1063.8, 0.2345, 9.15638050355709e9},
        {0.6597344572538565, 1063.8, 0.101, 1.232092207756679e10},
        {0.6597344572538565, 1063.8, 0.158, 1.078339233409766e10},
        {0.6597344572538565, 879.64, 0.2345, 7.571271428980038e9},
        {0.6597344572538565, 879.64, 0.101, 1.018798260604517e10},
        {0.6597344572538565, 879.64, 0.158, 8.91662270423545e9},
        {0.6597344572538565, 1101.27, 0.2345, 9.47889373674781e9},
        {0.6597344572538565, 1101.27, 0.101, 1.275489928216017e10},
        {0.6597344572538565, 1101.27, 0.158, 1.116321345720224e10}
    };

    int delta_days = 1;
    hypl::Atmosphere::AttenuationModel attenuation_model = hypl::Atmosphere::AttenuationModel::VB;

    for( int i = 0; i < nrows; i++)
    {
        double latitude = expected_values[i][0];
        double io = expected_values[i][1];
        double beta = expected_values[i][2];
        double expected_dni_yearly_energy = expected_values[i][3];

        hypl::Location location(latitude);
        hypl::Atmosphere atmosphere(io, beta, attenuation_model);
        hypl::Environment environment(location, atmosphere, delta_days);
        double computed_dni_yearly_energy =  hypl::auxfunction::DniYearlyEnergy(environment);

        double percentage_error = 100 * ( (computed_dni_yearly_energy/expected_dni_yearly_energy) - 1.0 );
        EXPECT_NEAR(percentage_error, 0.0, 0.25) << "i = " << i;
    }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}