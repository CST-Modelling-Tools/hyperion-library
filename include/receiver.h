#ifndef RECEIVER_H
#define RECEIVER_H

#include "export.h"
#include "vec3d.h"

namespace hypl
{
    class HYPERIONLIBRARY_API Receiver
    {
    public:
        Receiver() {}
        Receiver(vec3d aiming_point) : m_aiming_point{aiming_point}, m_power{0.0}, m_radius{-1.0}  {}
        Receiver(vec3d aiming_point, double radius) : m_aiming_point{aiming_point}, m_power{0.0}, m_radius{radius} {}

        //Accessors
        vec3d const& aiming_point() const { return m_aiming_point; }
        double const& power() const { return m_power; }
        double const& radius() const { return m_radius; }

        //Mutators
        void set_aiming_point(vec3d aiming_point) {m_aiming_point = aiming_point;}
        void set_power(double power) {m_power = power;}
        void set_radius(double radius) {m_radius = radius;}

        //Public functions
        void AddPower(double power_increase) { m_power += power_increase; }

    private:
        vec3d m_aiming_point;
        double m_power;
        double m_radius;
    };
}
#endif // RECEIVER_H