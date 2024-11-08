#include "vec2d.h"
#include "auxfunction.h"

namespace hypl {

    // Static member definitions - definitions without HYPERIONLIBRARY_API
    const vec2d Zero = vec2d(0.0, 0.0);
    const vec2d One = vec2d(1.0, 1.0);
    const vec2d UnitX = vec2d(1.0, 0.0);
    const vec2d UnitY = vec2d(0.0, 1.0);

    bool vec2d::operator==(const vec2d& v) const
    {
        if (this == &v) return true;

        return auxfunction::equals(x, v.x) && auxfunction::equals(y, v.y);
    }

    bool vec2d::operator!=(const vec2d& v) const
    {
        return !(*this == v);
    }

}

std::ostream& operator<<(std::ostream& os, const hypl::vec2d& v)
{
    os << v.x << ", " << v.y;
    return os;
}
