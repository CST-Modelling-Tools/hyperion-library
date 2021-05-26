#include <cmath>
#include "mathconstants.h"
#include "auxfunction.h"
#include "heliostatpowerperunitarea.h"

hypl::HeliostatPowerPerUnitArea::HeliostatPowerPerUnitArea(const Heliostat& heliostat, double declination)
    : m_heliostat {heliostat},
      m_declination {declination}
{
}

double hypl::HeliostatPowerPerUnitArea::operator()(double solar_time) const
{

    double hour_angle = solar_time * mathconstants::earth_rotational_speed;
    Eigen::Vector3d sun_vector = m_heliostat.location().SolarVector(hour_angle, m_declination);

    double ideal_efficiency = m_heliostat.Track(sun_vector).ideal_efficiency;
    double dni = m_heliostat.atmosphere().DniFromSz(sun_vector.z());
    return dni * ideal_efficiency;
}
