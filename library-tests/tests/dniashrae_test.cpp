#include "dniashrae_test.h"
#include "dniashrae.h"
#include "location.h"
#include "atmosphere.h"

using namespace hypl;


void dniashrae_test::constructors_accessors()
{


}

void dniashrae_test::mutators()
{

}

void dniashrae_test::public_functions()
{
    Atmosphere atm1;
    Location lct1;

    // DniASHRAE(double declination, const Location& location, const Atmosphere& atmosphere);
    DniASHRAE test(10,lct1,atm1);
   // 7.27221e-05
     //double operator()(double solar_time) const;
    QCOMPARE(test.operator()(1), 0);
}

// TO BE CONTINUED...
/*
    double Attenuation(double slant_range ) const;
    void Read(const QJsonObject& json);
    void Write(QJsonObject& json) const;
 */
