#ifndef MATHCONSTANTS_H
#define MATHCONSTANTS_H

#include "export.h"

namespace hypl
{
    namespace mathconstants
    {
        constexpr double pi = 3.141592653589793;
        constexpr double degree = pi/180.0;
        constexpr double earth_rotational_speed = (15.0*degree)/3600.0; // Radians per second
    } // namespace mathconstants
} 
#endif // MATHCONSTANTS_H
