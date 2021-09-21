#include <cmath>
#include "mathconstants.h"
#include "auxfunction.h"
#include "heliostatpowerperunitarea.h"

hypl::HeliostatPowerPerUnitArea::HeliostatPowerPerUnitArea(const Heliostat& heliostat, double declination, int day_number)
    : m_heliostat {heliostat},
      m_declination {declination},
      m_day_number {day_number}
{
}

double hypl::HeliostatPowerPerUnitArea::operator()(double solar_time) const
{

    double hour_angle = solar_time * mathconstants::earth_rotational_speed;
    vec3d sun_vector = m_heliostat.environment().location().SolarVector(hour_angle, m_declination);

    double ideal_efficiency = m_heliostat.Track(sun_vector, m_day_number).ideal_efficiency;
    double dni = m_heliostat.environment().atmosphere().DniFromSz(sun_vector.z);
    return dni * ideal_efficiency;
}