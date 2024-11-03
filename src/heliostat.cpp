#include <math.h>

#include "heliostat.h"
#include "auxfunction.h"

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


double hypl::Heliostat::ReceiverShadowing(int receiver_id, const vec3d& sun_vector) const
{
    vec3d ray_to_receiver_center = m_reflected_unit_vector[receiver_id] * m_slant_range[receiver_id];
    double aux_distance =  dot(sun_vector, ray_to_receiver_center);
    double sqrt_distance_sun_ray_to_receiver_center = m_slant_range[receiver_id] * m_slant_range[receiver_id] - aux_distance * aux_distance;

    double sqrt_receiver_radius = m_receivers[receiver_id].radius();
    sqrt_receiver_radius *= sqrt_receiver_radius;

    double shadow_factor = 0.0;
    if( sqrt_distance_sun_ray_to_receiver_center > sqrt_receiver_radius) shadow_factor = 1.0;

    return  shadow_factor;
}

hypl::Heliostat::TrackingInfo hypl::Heliostat::Track(const vec3d& sun_vector,  double sun_subtended_angle, IdealEfficiencyType ideal_efficiency_type) const
{
    TrackingInfo tracking_info = { -1.0, -1 };

    double cosine;
    double ideal_efficiency;
 
    switch ( ideal_efficiency_type )
    {
        case IdealEfficiencyType::CosineOnly:
            for (int receiver_id = 0; receiver_id < m_receivers.size(); receiver_id++)
            {
                ideal_efficiency = sqrt( 0.5 + 0.5 * dot(sun_vector, m_reflected_unit_vector[receiver_id]) );

                if (ideal_efficiency > tracking_info.ideal_efficiency)
                {
                    tracking_info.ideal_efficiency = ideal_efficiency;
                    tracking_info.aiming_at_receiver_id = receiver_id;
                }
            }
            break;

        case IdealEfficiencyType::CosineAndTransmittance:
            for (int receiver_id = 0; receiver_id < m_receivers.size(); receiver_id++)
            {            
                cosine = sqrt( 0.5 + 0.5 * dot(sun_vector, m_reflected_unit_vector[receiver_id]) );
                ideal_efficiency = cosine * m_transmittance[receiver_id];

                if (ideal_efficiency > tracking_info.ideal_efficiency)
                {
                    tracking_info.ideal_efficiency = ideal_efficiency;
                    tracking_info.aiming_at_receiver_id = receiver_id;
                }
            }           
            break;
            
        case IdealEfficiencyType::AllFactors:
            for (int receiver_id = 0; receiver_id < m_receivers.size(); receiver_id++)
            {
                cosine = sqrt( 0.5 + 0.5 * dot(sun_vector, m_reflected_unit_vector[receiver_id]) );
                ideal_efficiency = cosine * m_transmittance[receiver_id] *  Spillage(receiver_id, sun_subtended_angle) * ReceiverShadowing(receiver_id, sun_vector);

                if (ideal_efficiency > tracking_info.ideal_efficiency)
                {
                    tracking_info.ideal_efficiency = ideal_efficiency;
                    tracking_info.aiming_at_receiver_id = receiver_id;
                }
            }                      
            break;
    }


    return tracking_info;
}