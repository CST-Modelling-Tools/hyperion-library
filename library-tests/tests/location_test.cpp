#include "location_test.h"
#include "location.h"

using namespace hypl;

void Location_test::constructors_accessors()
{
    double latitude=100;

    Location lct1(latitude);

    QCOMPARE(lct1.latitude(),100);

    //mathconstants::degree = 0.0174533
    Location lct2;
    QCOMPARE(lct2.latitude(), mathconstants::degree * 37.2);

}

void Location_test::mutators()
{
    double latitude_degree = 100;
    Location lct1;

    lct1.set_latitude_degree(latitude_degree);


    QCOMPARE(lct1.LatitudeDegree(), 100);
}

void Location_test::public_functions()
{

}

// TO BE CONTINUED...
/*
        double HourAngleLimit(double declination) const;
        Eigen::Vector3d SolarVector(double hour_angle, double declination) const;
        void Read(const QJsonObject& json);
        void Write(QJsonObject& json) const;
 */

