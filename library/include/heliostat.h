#ifndef HELIOSTAT_H
#define HELIOSTAT_H

#include "HyperionLibrary_global.h"

#include <QVector>

//Eigen includes
#include <Eigen/Dense>

//Hyperion library includes
#include "location.h"
#include "atmosphere.h"
#include "receiver.h"

namespace hypl
{
    class HYPERIONLIBRARY_EXPORT Heliostat
    {

    public:
        struct TrackingInfo {
            double attenuation;
            double cosine_factor;
            double ideal_efficiency;
            int aiming_at_receiver_index;
        };

        Heliostat(Location& location, Atmosphere& atmosphere,
                  QVector<Receiver>& receivers, Eigen::Vector3d center);

        //Accessors
        const Location& location() const { return m_location; }
        const Atmosphere& atmosphere() const { return m_atmosphere; }
        const QVector<Receiver>& receivers() const { return m_receivers; }
        const Eigen::Vector3d& center() const { return m_center; }
        const QVector<double>& attenuation() const { return m_attenuation; }
        const QVector<double>& slant_range() const { return m_slant_range; }
        const QVector<Eigen::Vector3d>& reflected_unit_vector() const { return m_reflected_unit_vector; }

        //Mutators
        void update();
        void set_center(Eigen::Vector3d center) {m_center = center; update();}

        //Public functions
        TrackingInfo Track(Eigen::Vector3d& sun_vector) const;
        TrackingInfo Track(double hour_angle, double declination) const;
        double HeliostatDailyEnergyPerUnitArea(double declination);
        double HeliostatYearlyEnergyPerUnitArea(int delta_days);
        double HeliostatAnnualIdealEfficency(int delta_days, double dni_yearly_energy);

    private:
        Location& m_location;
        Atmosphere& m_atmosphere;
        QVector<Receiver>& m_receivers;
        Eigen::Vector3d m_center;
        QVector<double> m_attenuation;
        QVector<double> m_slant_range;
        QVector<Eigen::Vector3d> m_reflected_unit_vector;
    };
}
#endif // HELIOSTAT_H
