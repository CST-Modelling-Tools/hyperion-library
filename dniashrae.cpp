#include <cmath>
#include "mathconstants.h"
#include "dniashrae.h"

double hypl::DniASHRAE::operator()(double solar_time) const
{
    double hour_angle = mathconstants::earth_rotational_speed * solar_time;
    double latitude = m_environment.location().latitude();
    double sz = sin(m_declination) * sin(latitude) + cos(m_declination) * cos(latitude) * cos(hour_angle);

    return m_environment.atmosphere().DniFromSz(sz);
}
