#include "location.h"
#include "mathconstants.h"

double hypl::Location::HourAngleLimit(double declination) const
{
    return abs(acos(-tan(declination)*tan(m_latitude)));
}

hypl::vec3d hypl::Location::SolarVector(double hour_angle, double declination) const
{
    double cos_declination = cos(declination);
    double sin_declination = sin(declination);
    double cos_hour_angle = cos(hour_angle);

    return vec3d(
             -cos_declination * sin(hour_angle),
             sin_declination * m_cos_latitude - cos_declination * m_sin_latitude * cos_hour_angle,
             sin_declination * m_sin_latitude + cos_declination * m_cos_latitude * cos_hour_angle );
}
