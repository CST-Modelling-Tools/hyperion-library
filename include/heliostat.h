#ifndef HELIOSTAT_H
#define HELIOSTAT_H

#include "export.h"

#include <vector>

//Hyperion library includes
#include "environment.h"
#include "receiver.h"

#include "vec3d.h"

struct DLL_API std::_Container_base12;

template class DLL_API std::allocator<double>;
template class DLL_API std::_Vector_val<std::_Simple_types<double>>;
template class DLL_API std::_Compressed_pair<std::allocator<double>,std::_Vector_val<std::_Simple_types<double>>,true>;
template class DLL_API std::vector<double>;

template class DLL_API std::allocator<hypl::vec3d>;
template class DLL_API std::_Vector_val<std::_Simple_types<hypl::vec3d>>;
template class DLL_API std::_Compressed_pair<std::allocator<hypl::vec3d>,std::_Vector_val<std::_Simple_types<hypl::vec3d>>,true>;
template class DLL_API std::vector<hypl::vec3d>;

namespace hypl
{
    class DLL_API Heliostat
    {
 
    public:

        enum IdealEfficiencyType { CosineOnly, CosineAndTransmittance, AllFactors };

        struct TrackingInfo {
            double ideal_efficiency;            
            int aiming_at_receiver_id;
        };

        Heliostat(Environment& environment, std::vector<Receiver>& receivers, vec3d center);

        //Accessors
        const Environment& environment() const { return m_environment; }
        const std::vector<Receiver>& receivers() const { return m_receivers; }
        const vec3d& center() const { return m_center; }
        const std::vector<double>& transmittance() const { return m_transmittance; }
        const std::vector<double>& slant_range() const { return m_slant_range; }
        const std::vector<vec3d>& reflected_unit_vector() const { return m_reflected_unit_vector; }

        //Mutators
        void update();
        void set_center(vec3d center) { m_center = center; update(); }

        //Public functions
        TrackingInfo Track(vec3d& sun_vector, double sun_subtended_angle, IdealEfficiencyType ideal_efficiency_type) const;
        double Spillage(int receiver_id, double sun_subtended_angle) const;
        double ReceiverShadowing(int receiver_id, vec3d& sun_vector) const;

        double m_annual_ideal_efficiency;

    private:
        Environment& m_environment;
        std::vector<Receiver>& m_receivers;
        vec3d m_center;

        std::vector<double> m_transmittance;
        std::vector<double> m_slant_range;
        std::vector<vec3d> m_reflected_unit_vector;
    };
}
#endif // HELIOSTAT_H
