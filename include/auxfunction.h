#ifndef AUXFUNCTION_H
#define AUXFUNCTION_H

#include "export.h"
#include "environment.h"



namespace hypl
{
    namespace auxfunction
    {
        DLL_API int  DayNumber(int day_index);
        DLL_API double SolarDeclinationByDayNumber(int day_number);
        DLL_API double SolarDeclinationByIndex(int day_index);
        DLL_API double DniDailyEnergy(double declination, const hypl::Environment& environment);
        DLL_API double DniYearlyEnergy(const hypl::Environment& environment);
        DLL_API double Distance_earth_sun(int day_number);
        DLL_API double Subtended_angle(double distance_ratio);
    }
}
#endif // AUXFUNCTION_H
