#include <cmath>
#include "mathconstants.h"
#include "auxfunction.h"
#include "atmosphere.h"
#include "dniashrae.h"

hypl::DniASHRAE::DniASHRAE(double declination, const Location& location, const Atmosphere& atmosphere)
    : m_declination {declination},
      m_location {location},
      m_atmosphere { atmosphere }
{

}

double hypl::DniASHRAE::operator()(double solar_time) const
{
    double hour_angle = mathconstants::earth_rotational_speed * solar_time;
    double sz = sin(m_declination) * sin(m_location.latitude()) + cos(m_declination) * cos(m_location.latitude()) * cos(hour_angle);

    return m_atmosphere.DniFromSz(sz);
}
