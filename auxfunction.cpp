#include <algorithm>         //to call std:sort
#include "auxfunction.h"
#include "mathconstants.h"
#include "dniashrae.h"
#include "gcf.h"

int hypl::auxfunction::DayNumber(int day_index)
{
    return (day_index + 355) % 365;
}

double hypl::auxfunction::SolarDeclinationByDayNumber(int day_number)
{
    const double k = (2. * mathconstants::pi)/365.;
    double omega = k * (day_number - 1.);

    double declination = 0.006918 - 0.399912 * cos(omega) + 0.070257 * sin(omega)
                                  - 0.006758 * cos(2.*omega) + 0.000907 * sin(2.*omega)
                                  - 0.002697 * cos(3.*omega) + 0.001480 * sin(3.*omega);
    return declination;
}

double hypl::auxfunction::SolarDeclinationByIndex(int day_index)
{
    return SolarDeclinationByDayNumber(DayNumber(day_index));
}

double hypl::auxfunction::Distance_earth_sun(int day_number)
{
    double average_distance = 149597870.0; //in kilometers
    double beta = (gcf::TwoPi * day_number) / 365.;
    double two_beta = 2.0 * beta;
    double aux = 1.00011 + 0.034221 * cos(beta) + 0.00128 * sin(beta) + 0.000719 * cos(two_beta) + 0.000077 * sin(two_beta);

    return sqrt( average_distance * (average_distance / aux) );
}

double hypl::auxfunction:: Subtended_angle(double ratio_distance)
{
    return ( gcf::TwoPi * (1.0 - sqrt(1.0 - ratio_distance*ratio_distance)));
}

