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
        DLL_API double DniDailyEnergy(double declination, const hypl::Environment& environment);
        DLL_API double DniYearlyEnergy(const hypl::Environment& environment);
        DLL_API double DistanceSunEarth(int day_number);
        DLL_API double ReceiverSubtendedAngle(double distance_ratio);
    }
}
#endif // AUXFUNCTION_H
