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
#include "gcf.h"


hypl::Heliostat::Heliostat(Environment& environment, std::vector<Receiver>& receivers, vec3d center)
    : m_environment {environment},
      m_receivers {receivers},
      m_center {center} { update(); }

void hypl::Heliostat::update()
{
    if( m_slant_range.empty() == false ) m_slant_range.clear();
    if( m_attenuation.empty() == false ) m_attenuation.clear();
    if( m_reflected_unit_vector.empty() == false ) m_reflected_unit_vector.clear();

    int nreceivers = m_receivers.size();
    m_slant_range.reserve(nreceivers);
    m_attenuation.reserve(nreceivers);
    m_reflected_unit_vector.reserve(nreceivers);

    vec3d aux_vector3d;
    double slant_range = 0.0;
    for (auto& element : m_receivers)
    {
        aux_vector3d = element.aiming_point() - m_center;
        slant_range = aux_vector3d.norm();
        m_reflected_unit_vector.emplace_back(aux_vector3d.normalized());
        m_slant_range.emplace_back(slant_range);
        m_attenuation.emplace_back(m_environment.atmosphere().Attenuation(slant_range));
    }

    m_annual_energy_per_unit_area = -999.0; //mark for lazy evaluation
    m_annual_ideal_efficiency = -999.0; //mark for lazy evaluation
}


double hypl::Heliostat::get_spillage(int receiver_id, int day_number) const
{

    double sun_subtended_angle = m_environment.get_sun_subtended_angle(day_number);
    double receiver_subtended_angle = auxfunction::Subtended_angle(m_receivers[receiver_id].radius() / m_slant_range[receiver_id]);

    if( receiver_subtended_angle < sun_subtended_angle) return receiver_subtended_angle / sun_subtended_angle;

    return 1.0;
}


double hypl::Heliostat::get_receiver_shading(vec3d& sun_vector, int receiver_id) const
{
    vec3d heliostat_tower_vector = m_reflected_unit_vector[receiver_id] * m_slant_range[receiver_id];
    double aux_distance =  dot(sun_vector,heliostat_tower_vector);
    double sqrt_distance_ray_receiver_center = m_slant_range[receiver_id] * m_slant_range[receiver_id] - aux_distance * aux_distance;

    double sqrt_receiver_radius = m_receivers[receiver_id].radius();
    sqrt_receiver_radius *= sqrt_receiver_radius;

    double shadow_factor = 0.0;
    if( sqrt_distance_ray_receiver_center > sqrt_receiver_radius) shadow_factor = 1.0;

    return  shadow_factor;
}

hypl::Heliostat::TrackingInfo hypl::Heliostat::Track(vec3d& sun_vector, int day_number) const
{
    TrackingInfo tracking_info = { -1.0, -1.0, -1.0, 1.0, 1.0, -1 };

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

    int receiver_id = tracking_info.aiming_at_receiver_index;
    if (m_receivers[receiver_id].radius() > 0.)
    {
        tracking_info.spillage = get_spillage(receiver_id, day_number);
        tracking_info.receiver_shadow = get_receiver_shading(sun_vector, receiver_id);
        tracking_info.ideal_efficiency *= (tracking_info.receiver_shadow * tracking_info.spillage);
    }

    return tracking_info;
}

hypl::Heliostat::TrackingInfo hypl::Heliostat::Track(double hour_angle, double declination, int day_number) const
{
    vec3d sun_vector = m_environment.location().SolarVector(hour_angle, declination);
    return Track(sun_vector, day_number);
}

double hypl::Heliostat::HeliostatDailyEnergyPerUnitArea(int day_number)
{
    double declination = m_environment.get_declination(day_number);
    double wo = m_environment.location().HourAngleLimit(declination);
    double t_start = -wo/mathconstants::earth_rotational_speed;
    double t_end = -t_start;

    HeliostatPowerPerUnitArea heliostatPowerPerUnitArea(*this, declination, day_number);
    return DEIntegrator<HeliostatPowerPerUnitArea>::Integrate(heliostatPowerPerUnitArea, t_start, t_end, 1e-4);
}

/*
const double hypl::Heliostat::annual_energy_per_unit_area()
{
    if( m_annual_energy_per_unit_area < 0.0)
    {
        double sum = 0.0;
        int day_index = 1;
        int delta_days = m_environment.delta_days();
        while (day_index < 366)
        {
            m_sun_subtended_angle = auxfunction::Subtended_angle(gcf::Sun_radius / auxfunction::Distance_earth_sun(day_index));
            sum += HeliostatDailyEnergyPerUnitArea(auxfunction::SolarDeclinationByIndex(day_index));
            day_index += delta_days;
        }
        m_annual_energy_per_unit_area = sum;
    }
    return  m_annual_energy_per_unit_area;
}
*/


const double hypl::Heliostat::annual_energy_per_unit_area()
{
    if( m_annual_energy_per_unit_area < 0.0)
    {
        double sum = 0.0;
        int day_number = 174;
        while (day_number < 356)
        {
            sum += HeliostatDailyEnergyPerUnitArea(day_number);
            day_number++;
        }
        m_annual_energy_per_unit_area = 2.0 * sum + 0.5 * HeliostatDailyEnergyPerUnitArea(173);
    }
    return  m_annual_energy_per_unit_area;
}


const double hypl::Heliostat::annual_ideal_efficiency()
{
    if( m_annual_ideal_efficiency < 0.0) m_annual_ideal_efficiency = annual_energy_per_unit_area() / m_environment.dni_annual_energy();
    return m_annual_ideal_efficiency;
}