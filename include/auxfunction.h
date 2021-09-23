#ifndef AUXFUNCTION_H
#define AUXFUNCTION_H

#include "export.h"

namespace hypl
{
    namespace auxfunction
    {
        DLL_API double SolarDeclinationByDayNumber(int day_number);
        DLL_API double DistanceSunEarth(int day_number);
        DLL_API double ReceiverSubtendedAngle(double ratio_of_distances );
    }
}
#endif // AUXFUNCTION_H
