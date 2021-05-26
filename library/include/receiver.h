#ifndef RECEIVER_H
#define RECEIVER_H

#include "HyperionLibrary_global.h"

#include <Eigen/Dense>
#include <QJsonObject>

namespace hypl
{
    class HYPERIONLIBRARY_EXPORT Receiver
    {
    public:
        Receiver() {}
        Receiver(Eigen::Vector3d aiming_point) : m_aiming_point{aiming_point}, m_power{0.0} {}

        //Accessors
        Eigen::Vector3d aiming_point() const { return m_aiming_point; }
        double power() const { return m_power; }

        //Mutators
        void set_aiming_point(Eigen::Vector3d aiming_point) {m_aiming_point = aiming_point;}
        void set_power(double power) {m_power = power;}

        //Public functions
        void AddPower(double power_increase) { m_power += power_increase; }
        void Read(const QJsonObject& json);
        void Write(QJsonObject& json) const;

    private:
        Eigen::Vector3d m_aiming_point;
        double m_power;
    };
}
#endif // RECEIVER_H
