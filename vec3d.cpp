#include "gcf.h"

#include "vec3d.h"

const hypl::vec3d hypl::vec3d::Zero(0., 0., 0.);
const hypl::vec3d hypl::vec3d::One(1., 1., 1.);
const hypl::vec3d hypl::vec3d::UnitX(1., 0., 0.);
const hypl::vec3d hypl::vec3d::UnitY(0., 1., 0.);
const hypl::vec3d hypl::vec3d::UnitZ(0., 0., 1.);

bool hypl::vec3d::operator==(const vec3d& v) const
{
    return gcf::equals(x, v.x) &&
           gcf::equals(y, v.y) &&
           gcf::equals(z, v.z);
}

bool hypl::vec3d::operator!=(const vec3d& v) const
{
    return !(*this == v);
}

int hypl::vec3d::maxDimension() const
{
    if (x > y && x > z)
        return 0;
    else if (y > z)
        return 1;
    return 2;
}

hypl::vec3d hypl::vec3d::directionAE(double azimuth, double elevation)// in radians
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
