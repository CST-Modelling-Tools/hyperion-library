#include "auxfunction_test.h"
#include "auxfunction.h"

using namespace hypl;

void Auxfunction_test::constructors_accessors()
{

}

void Auxfunction_test::mutators()
{

}

void Auxfunction_test::public_functions()
{

     Eigen::Vector3d a(1,1,1);
     Eigen::Vector3d b(1,1,1);
     Eigen::Vector3d c(1,1,1);


    QCOMPARE(auxfunction::DayNumber(10), 0);
    //pi = 3.14159, day_number = 1, omega = 0
    QCOMPARE(auxfunction::SolarDeclinationByDayNumber(1), -0.402449);
    QCOMPARE(auxfunction::SolarDeclinationByIndex(11), -0.402449);
}


// TO BE CONTINUED...

/*
        double HYPERIONLIBRARY_EXPORT DniDailyEnergy(double declination, const Location &location, const Atmosphere &atmosphere);
        double HYPERIONLIBRARY_EXPORT DniYearlyEnergy(const Location& location, const Atmosphere& atmosphere, int delta_days);
 */


