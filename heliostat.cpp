#include <math.h>
#include <vector>
#include <thread>
#include <future>

#include "location.h"
#include "atmosphere.h"
#include "receiver.h"
#include "heliostat.h"
#include "auxfunction.h"
#include "gcf.h"


hypl::Heliostat::Heliostat(Environment& environment, std::vector<Receiver>& receivers, vec3d center) : 
m_environment {environment},
m_receivers {receivers},
m_center {center} 
{ update(); }

void hypl::Heliostat::update()
{
    if( m_slant_range.empty() == false ) m_slant_range.clear();
    if( m_transmittance.empty() == false ) m_transmittance.clear();
    if( m_reflected_unit_vector.empty() == false ) m_reflected_unit_vector.clear();

    int nreceivers = m_receivers.size();
    m_slant_range.reserve(nreceivers);
    m_transmittance.reserve(nreceivers);
    m_reflected_unit_vector.reserve(nreceivers);

    vec3d aux_vector3d;
    for (auto& element : m_receivers)
    {
        aux_vector3d = element.aiming_point() - m_center;
        double slant_range = aux_vector3d.norm();
        m_slant_range.emplace_back(slant_range);
        m_reflected_unit_vector.emplace_back(aux_vector3d.normalized());
        m_transmittance.emplace_back(m_environment.atmosphere().Transmittance(slant_range));
    }

    m_annual_ideal_efficiency = 0.0;
}


double hypl::Heliostat::Spillage(int receiver_id, double sun_subtended_angle) const
{
    double receiver_subtended_angle = auxfunction::ReceiverSubtendedAngle(m_receivers[receiver_id].radius() / m_slant_range[receiver_id]);

    if( receiver_subtended_angle < sun_subtended_angle) return receiver_subtended_angle / sun_subtended_angle;
    else return 1.0;
}


double hypl::Heliostat::ReceiverShadowing(int receiver_id, vec3d& sun_vector ) const
{
    vec3d heliostat_tower_vector = m_reflected_unit_vector[receiver_id] * m_slant_range[receiver_id];
    double aux_distance =  dot(sun_vector, heliostat_tower_vector);
    double sqrt_distance_ray_receiver_center = m_slant_range[receiver_id] * m_slant_range[receiver_id] - aux_distance * aux_distance;

    double sqrt_receiver_radius = m_receivers[receiver_id].radius();
    sqrt_receiver_radius *= sqrt_receiver_radius;

    double shadow_factor = 0.0;
    if( sqrt_distance_ray_receiver_center > sqrt_receiver_radius) shadow_factor = 1.0;

    return  shadow_factor;
}

hypl::Heliostat::TrackingInfo hypl::Heliostat::Track(vec3d& sun_vector,  double sun_subtended_angle, IdealEfficiencyType ideal_efficiency_type) const
{
    TrackingInfo tracking_info = { -1.0, -1 };

    double cosine;
    double transmittance;
    double spillage;
    double receiver_shadowing;
    double ideal_efficiency;

    for (int receiver_id = 0; receiver_id < m_receivers.size(); receiver_id++)
    {
        switch ( ideal_efficiency_type )
        {
            case IdealEfficiencyType::CosineOnly:
                cosine = sqrt( 0.5 * (1.0 + dot(sun_vector, m_reflected_unit_vector[receiver_id]) ) );
                ideal_efficiency = cosine;
                break;
            case IdealEfficiencyType::CosineAndTransmittance:
                cosine = sqrt( 0.5 * (1.0 + dot(sun_vector, m_reflected_unit_vector[receiver_id]) ) );
                transmittance = m_transmittance[receiver_id];
                ideal_efficiency = cosine * transmittance;
                break;
            case IdealEfficiencyType::AllFactors:
                cosine = sqrt( 0.5 * (1.0 + dot(sun_vector, m_reflected_unit_vector[receiver_id]) ) );
                transmittance = m_transmittance[receiver_id];
                spillage = Spillage(receiver_id, sun_subtended_angle);
                receiver_shadowing = ReceiverShadowing(receiver_id, sun_vector);
                ideal_efficiency = cosine * transmittance * spillage * receiver_shadowing;           
                break;
            default:
                ideal_efficiency = -1.0;
                break;
        }

        if (ideal_efficiency > tracking_info.ideal_efficiency)
        {
            tracking_info.ideal_efficiency = ideal_efficiency;
            tracking_info.aiming_at_receiver_id = receiver_id;
        }
    }

    return tracking_info;
}