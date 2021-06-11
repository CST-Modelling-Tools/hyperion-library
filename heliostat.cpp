#include <math.h>
#include <vector>
#include <thread>
#include <future>

#include "location.h"
#include "atmosphere.h"
#include "receiver.h"
#include "heliostat.h"
#include "auxfunction.h"
#include "heliostatpowerperunitarea.h"
#include "deintegrator.h"


hypl::Heliostat::Heliostat(Environment& environment, std::vector<Receiver>& receivers, vec3d center)
    : m_environment {environment},
      m_receivers {receivers},
      m_center {center} { update(); }

void hypl::Heliostat::update()
{
    if( m_slant_range.empty() == false ) m_slant_range.clear();
    if( m_attenuation.empty() == false ) m_attenuation.clear();
    if( m_reflected_unit_vector.empty() == false ) m_reflected_unit_vector.clear();

    vec3d aux_vector3d;
    double slant_range = 0.0;
    for (auto& element : m_receivers)
    {
        aux_vector3d = element.aiming_point() - m_center;
        slant_range = aux_vector3d.norm();
        m_reflected_unit_vector.push_back(aux_vector3d.normalized());
        m_slant_range.push_back(slant_range);
        m_attenuation.push_back(m_environment.atmosphere().Attenuation(slant_range));
    }

    m_annual_energy_per_unit_area = -999.0; //mark for lazy evaluation
    m_annual_ideal_efficiency = -999.0; //mark for lazy evaluation
}

hypl::Heliostat::TrackingInfo hypl::Heliostat::Track(vec3d& sun_vector) const
{
    TrackingInfo tracking_info = { -1.0, -1.0, -1.0, -1 };

    double heliostat_cosine = -1.0;
    double ideal_efficiency = -1.0;

    for (int i = 0; i< m_reflected_unit_vector.size(); i++)
    {
        heliostat_cosine = sqrt( 0.5 * (1.0 + dot(sun_vector, m_reflected_unit_vector[i]) ) );
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
    vec3d sun_vector = m_environment.location().SolarVector(hour_angle, declination);
    return Track(sun_vector);
}

double hypl::Heliostat::HeliostatDailyEnergyPerUnitArea(double declination)
{
    double wo = m_environment.location().HourAngleLimit(declination);
    double t_start = -wo/mathconstants::earth_rotational_speed;
    double t_end = -t_start;

    HeliostatPowerPerUnitArea heliostatPowerPerUnitArea(*this, declination);
    return DEIntegrator<HeliostatPowerPerUnitArea>::Integrate(heliostatPowerPerUnitArea, t_start, t_end, 1e-4);
}

const double hypl::Heliostat::annual_energy_per_unit_area()
{
    if( m_annual_energy_per_unit_area < 0.0)
    {
        double sum = 0.0;
        int day_index = 1;
        int delta_days = m_environment.delta_days();
        while (day_index < 366)
        {
            sum += HeliostatDailyEnergyPerUnitArea(auxfunction::SolarDeclinationByIndex(day_index));
            day_index += delta_days;
        }
        m_annual_energy_per_unit_area = sum;
    }
    return  m_annual_energy_per_unit_area;
}


const double hypl::Heliostat::annual_ideal_efficiency()
{
    if( m_annual_ideal_efficiency < 0.0) m_annual_ideal_efficiency = annual_energy_per_unit_area() / m_environment.dni_annual_energy();
    return m_annual_ideal_efficiency;
}