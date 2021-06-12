#include <algorithm>         //to call std:sort
#include "auxfunction.h"
#include "mathconstants.h"
#include "dniashrae.h"
#include "deintegrator.h"

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


double hypl::auxfunction::DniDailyEnergy(double declination, const Environment& environment)
{
    double wo = environment.location().HourAngleLimit(declination);
    double t_start = -wo/mathconstants::earth_rotational_speed;
    double t_end = 0.0; //Considering a symmetric DNI curve around true solar noon.

    DniASHRAE dni(declination, environment);
    return 2.0 * DEIntegrator<DniASHRAE>::Integrate(dni, t_start, t_end, 1e-12);
}

double hypl::auxfunction::DniYearlyEnergy(const Environment& environment)
{
    double sum = 0.0;
    int day_index = 1;
    int delta_days = environment.delta_days();
    while (day_index < 183)
    {
        sum += DniDailyEnergy(SolarDeclinationByIndex(day_index), environment);
        day_index += delta_days;
    }
    return (2.0*sum)+DniDailyEnergy(SolarDeclinationByIndex(183), environment);
}
