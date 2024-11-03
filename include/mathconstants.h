#ifndef MATHCONSTANTS_H
#define MATHCONSTANTS_H

#include <numbers> // For std::numbers::pi
#include "export.h"

namespace hypl
{
    namespace mathconstants
    {
        inline constexpr double pi = std::numbers::pi;
        inline constexpr double two_pi = 2.0 * pi;
        inline constexpr double degree = pi/180.0;
        inline constexpr double earth_rotational_speed = (15.0*degree)/3600.0; // Radians per second
        inline constexpr double astronomical_unit = 149597870.7; //in kilometers
        inline constexpr double sun_radius = 695700.0; //kilometers
    } // namespace mathconstants
} 
#endif // MATHCONSTANTS_H