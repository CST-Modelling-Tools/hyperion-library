#ifndef RECEIVER_H
#define RECEIVER_H

#include "export.h"
#include "vec3d.h"

namespace hypl
{
    class DLL_API Receiver
    {
    public:
        Receiver() {}
        Receiver(vec3d aiming_point) : m_aiming_point{aiming_point}, m_power{0.0} {}

        //Accessors
        vec3d aiming_point() const { return m_aiming_point; }
        double power() const { return m_power; }

        //Mutators
        void set_aiming_point(vec3d aiming_point) {m_aiming_point = aiming_point;}
        void set_power(double power) {m_power = power;}

        //Public functions
        void AddPower(double power_increase) { m_power += power_increase; }

    private:
        vec3d m_aiming_point;
        double m_power;
    };
}
#endif // RECEIVER_H