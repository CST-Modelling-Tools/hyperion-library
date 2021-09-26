#ifndef MATHCONSTANTS_H
#define MATHCONSTANTS_H

#include "export.h"

namespace hypl
{
    namespace mathconstants
    {
        constexpr double pi = 3.14159265358979323846264338327950288;
        constexpr double two_pi = 2.0 * pi;
        constexpr double degree = pi/180.0;
        constexpr double earth_rotational_speed = (15.0*degree)/3600.0; // Radians per second
        constexpr double astronomical_unit = 149597870.7; //in kilometers
        constexpr double sun_radius = 695700.0; //kilometers
    } // namespace mathconstants
} 
#endif // MATHCONSTANTS_H
