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
    if( m_shading_limit.empty() == false ) m_shading_limit.clear();
    if( m_receiver_subtended_angle.empty() == false ) m_receiver_subtended_angle.clear();
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
        m_receiver_subtended_angle.push_back(999.0);
        m_shading_limit.push_back(get_receiver_shading_limit(element, slant_range));
    }

    m_annual_energy_per_unit_area = -999.0; //mark for lazy evaluation
    m_annual_ideal_efficiency = -999.0; //mark for lazy evaluation
}

void hypl::Heliostat::update_receivers_parameters()
{
    if ( m_shading_limit.empty() == false ) m_shading_limit.clear();
    if ( m_receiver_subtended_angle.empty() == false ) m_receiver_subtended_angle.clear();

    for (int i = 0; i< m_receivers.size(); i++)
    {
        m_shading_limit.push_back(get_receiver_shading_limit(m_receivers[i], m_slant_range[i]));
        m_receiver_subtended_angle.push_back(get_receiver_subtended_angle(m_receivers[i], m_slant_range[i]));
    }
}

double hypl::Heliostat::get_receiver_shading_limit(Receiver& receiver, double slant_range) const
{
    double receiver_radius = receiver.radius();

    if ( receiver_radius <= 0. ) return 999.0;

    vec2d tangent_point(receiver_radius,sqrt(slant_range*slant_range - receiver_radius*receiver_radius));
    tangent_point *= (receiver_radius / slant_range);
    vec2d heliostat_point(slant_range,0.);
    vec2d vector_tangent = tangent_point - heliostat_point;
    
    double cos_angle_limit = dot(vector_tangent,-heliostat_point) / (slant_range * vector_tangent.norm());

    return  sqrt((cos_angle_limit + 1) / 2.0 );
}

double hypl::Heliostat::get_receiver_subtended_angle(Receiver& receiver, double slant_range) const
{
    if ( receiver.radius() <= 0. ) return 999.0;

    double ratio_distance = receiver.radius() / slant_range;
    return ( gcf::TwoPi * (1.0 - sqrt(1.0 - ratio_distance*ratio_distance)));
}

hypl::Heliostat::TrackingInfo hypl::Heliostat::Track(vec3d& sun_vector) const
{
    TrackingInfo tracking_info = { -1.0, -1.0, -1.0, -1.0, -1 };

    double heliostat_cosine = -1.0;
    double ideal_efficiency = -1.0;
    double receiver_shadow = -1.0;

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

    if( tracking_info.cosine_factor < m_shading_limit[tracking_info.aiming_at_receiver_index]) tracking_info.receiver_shadow = 1.0;
    else  tracking_info.receiver_shadow = 0.0;
    tracking_info.ideal_efficiency *= tracking_info.receiver_shadow;

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

/*
const double hypl::Heliostat::annual_energy_per_unit_area()
{
    if( m_annual_energy_per_unit_area < 0.0)
    {
        double sum = 0.0;
        int day_number = 1;
        while (day_number < 366)
        {
            // To save time, calculate HERE the angle subtended by the sun here AND the spillage factor, as they are only function of day number
            sum += HeliostatDailyEnergyPerUnitArea(auxfunction::SolarDeclinationByDayNumber(day_number));
            day_number++;
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
            // To save time, calculate HERE the angle subtended by the sun here AND the spillage factor, as they are only function of day number
            sum += HeliostatDailyEnergyPerUnitArea(auxfunction::SolarDeclinationByDayNumber(day_number));
            day_number++;
        }
        m_annual_energy_per_unit_area = 2.0 * sum + 0.5 * HeliostatDailyEnergyPerUnitArea(auxfunction::SolarDeclinationByDayNumber(173));
    }
    return  m_annual_energy_per_unit_area;
}


const double hypl::Heliostat::annual_ideal_efficiency()
{
    if( m_annual_ideal_efficiency < 0.0) m_annual_ideal_efficiency = annual_energy_per_unit_area() / m_environment.dni_annual_energy();
    return m_annual_ideal_efficiency;
}