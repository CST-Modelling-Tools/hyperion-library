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
    constexpr double k = (2.0 * mathconstants::pi)/365.;

    double omega = k * (day_number - 1);
    double two_omega = 2.0 * omega;
    double three_omega = 3.0 * omega;

    double declination = 0.006918 - 0.399912 * cos(omega) + 0.070257 * sin(omega)
                                  - 0.006758 * cos(two_omega) + 0.000907 * sin(two_omega)
                                  - 0.002697 * cos(three_omega) + 0.001480 * sin(three_omega);
    return declination;
}

double hypl::auxfunction::DistanceSunEarth(int day_number)
{   
    constexpr double k = (mathconstants::two_pi)/365.0;
    constexpr double square_of_astronomical_unit =  mathconstants::astronomical_unit *  mathconstants::astronomical_unit;

    double omega = k * (day_number-1);
    double two_omega = 2.0 * omega;

    double one_over_r2 = 1.00011 + 0.034221 * cos(omega) + 0.00128 * sin(omega) + 0.000719 * cos(two_omega) + 0.000077 * sin(two_omega);

    return sqrt( square_of_astronomical_unit / one_over_r2 );
}

double hypl::auxfunction:: ReceiverSubtendedAngle(double ratio_of_distances ) 
{
    return ( mathconstants::two_pi * ( 1.0 - sqrt(1.0 - (ratio_of_distances * ratio_of_distances ) ) ) );
}

