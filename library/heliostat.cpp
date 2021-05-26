#include <QDebug>
#include <math.h>

#include "heliostat.h"
#include "auxfunction.h"
#include "heliostatpowerperunitarea.h"
#include "deintegrator.h"

hypl::Heliostat::Heliostat(Location& location, Atmosphere& atmosphere,
                           QVector<Receiver>& receivers, Eigen::Vector3d center)
    : m_location {location},
      m_atmosphere {atmosphere},
      m_receivers {receivers},
      m_center {center} { update(); }

void hypl::Heliostat::update()
{
    if( m_slant_range.isEmpty() == false ) m_slant_range.clear();
    if( m_attenuation.isEmpty() == false ) m_attenuation.clear();
    if( m_reflected_unit_vector.isEmpty() == false ) m_reflected_unit_vector.clear();

    Eigen::Vector3d aux_vector3d;
    double slant_range = 0.0;
    for (auto& element : m_receivers)
    {
        aux_vector3d = element.aiming_point() - m_center;
        slant_range = aux_vector3d.norm();
        m_reflected_unit_vector.append(aux_vector3d.normalized());
        m_slant_range.append(slant_range);
        m_attenuation.append(m_atmosphere.Attenuation(slant_range));
    }
}

hypl::Heliostat::TrackingInfo hypl::Heliostat::Track(Eigen::Vector3d& sun_vector) const
{
    TrackingInfo tracking_info = { -1.0, -1.0, -1.0, -1 };

    double heliostat_cosine = -1.0;
    double ideal_efficiency = -1.0;

    for (int i = 0; i< m_reflected_unit_vector.size(); i++)
    {
        heliostat_cosine = sqrt( 0.5*(1.0 + sun_vector.dot(m_reflected_unit_vector[i])));
        ideal_efficiency = heliostat_cosine * m_attenuation[i];

        if (ideal_efficiency > tracking_info.ideal_efficiency)
        {
            tracking_info.attenuation = m_attenuation[i];
            tracking_info.cosine_factor = heliostat_cosine;
            tracking_info.ideal_efficiency = ideal_efficiency;
            tracking_info.aiming_at_receiver_index = i;
        }
    }
    return tracking_info;
}

hypl::Heliostat::TrackingInfo hypl::Heliostat::Track(double hour_angle, double declination) const
{
    Eigen::Vector3d sun_vector = m_location.SolarVector(hour_angle, declination);
    return Track(sun_vector);
}

double hypl::Heliostat::HeliostatDailyEnergyPerUnitArea(double declination)
{
    double wo = m_location.HourAngleLimit(declination);
    double t_start = -wo/mathconstants::earth_rotational_speed;
    double t_end = -t_start;

    HeliostatPowerPerUnitArea heliostatPowerPerUnitArea(*this, declination);
    return DEIntegrator<HeliostatPowerPerUnitArea>::Integrate(heliostatPowerPerUnitArea, t_start, t_end, 1e-4);
}

double hypl::Heliostat::HeliostatYearlyEnergyPerUnitArea(int delta_days)
{
    double sum = 0.0;
    int day_index = 1;
    while (day_index < 366)
    {
        sum += HeliostatDailyEnergyPerUnitArea(auxfunction::SolarDeclinationByIndex(day_index));
        day_index += delta_days;
    }
    return sum;
}

double hypl::Heliostat::HeliostatAnnualIdealEfficency(int delta_days, double dni_yearly_energy)
{
    return HeliostatYearlyEnergyPerUnitArea(delta_days) / dni_yearly_energy;
}
