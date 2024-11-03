#include "vec2d.h"

#include "auxfunction.h"

namespace hypl {
    constexpr vec2d vec2d::Zero{0.0, 0.0};
    constexpr vec2d vec2d::One{1.0, 1.0};
    constexpr vec2d vec2d::UnitX{1.0, 0.0};
    constexpr vec2d vec2d::UnitY{0.0, 1.0};
}

bool hypl::vec2d::operator==(const vec2d& v) const
{
    if (this == &v) return true;

    return auxfunction::equals(x, v.x) && auxfunction::equals(y, v.y);
}

bool hypl::vec2d::operator!=(const vec2d& v) const
{
    return !(*this == v);
}

std::ostream& operator<<(std::ostream& os, const hypl::vec2d& v)
{
    os << v.x << ", " << v.y;
    return os;
}