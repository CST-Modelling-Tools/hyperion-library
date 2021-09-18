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
        struct TrackingInfo {
            double attenuation;
            double cosine_factor;
            double receiver_shadow;
            double ideal_efficiency;
            int aiming_at_receiver_index;
        };

        Heliostat(Environment& environment, std::vector<Receiver>& receivers, vec3d center);

        //Accessors
        const Environment& environment() const { return m_environment; }
        const std::vector<Receiver>& receivers() const { return m_receivers; }
        const vec3d& center() const { return m_center; }
        const std::vector<double>& attenuation() const { return m_attenuation; }
        const std::vector<double>& slant_range() const { return m_slant_range; }
        const std::vector<double>& shading_limit() const { return m_shading_limit; }
        const std::vector<double>& receiver_subtended_angle() const { return m_receiver_subtended_angle; }
        const std::vector<vec3d>& reflected_unit_vector() const { return m_reflected_unit_vector; }
        const double annual_energy_per_unit_area();
        const double annual_ideal_efficiency();

        //Mutators
        void update();
        void update_receivers_parameters();
        void set_center(vec3d center) {m_center = center; update();}

        //Public functions
        TrackingInfo Track(vec3d& sun_vector) const;
        TrackingInfo Track(double hour_angle, double declination) const;
        double HeliostatDailyEnergyPerUnitArea(double declination);
        double get_receiver_shading_limit(Receiver& receiver, double slant_range) const;
        double get_receiver_subtended_angle(Receiver& receiver, double slant_range) const;

    private:
        Environment& m_environment;
        std::vector<Receiver>& m_receivers;
        vec3d m_center;

        std::vector<double> m_attenuation;
        std::vector<double> m_slant_range;
        std::vector<double> m_shading_limit;
        std::vector<double> m_receiver_subtended_angle;
        std::vector<vec3d> m_reflected_unit_vector;

        double m_annual_energy_per_unit_area;
        double m_annual_ideal_efficiency;
    };
}
#endif // HELIOSTAT_H
