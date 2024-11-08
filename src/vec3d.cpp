#include "auxfunction.h"
#include "vec3d.h"

namespace hypl {

    // Static member definitions - definitions without HYPERIONLIBRARY_API
    const vec3d Zero = vec3d(0., 0., 0.);
    const vec3d One = vec3d(1., 1., 1.);
    const vec3d UnitX = vec3d(1., 0., 0.);
    const vec3d UnitY = vec3d(0., 1., 0.);
    const vec3d UnitZ = vec3d(0., 0., 1.);


    bool vec3d::operator==(const vec3d& v) const
    {
        return auxfunction::equals(x, v.x) &&
            auxfunction::equals(y, v.y) &&
            auxfunction::equals(z, v.z);
    }

    bool vec3d::operator!=(const vec3d& v) const
    {
        return !(*this == v);
    }

    int vec3d::maxDimension() const
    {
        if (x > y && x > z)
            return 0;
        else if (y > z)
            return 1;
        return 2;
    }

    hypl::vec3d vec3d::directionAE(double azimuth, double elevation)// in radians
    {
        double cosAlpha = cos(elevation);
        return vec3d(
            cosAlpha*sin(azimuth),
            cosAlpha*cos(azimuth),
            sin(elevation)
        );
    }

    std::ostream& operator<<(std::ostream& os, const hypl::vec3d& v)
    {
        os << v.x << ", " << v.y << ", " << v.z;
        return os;
    }
}

