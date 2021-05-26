#ifndef LOCATION_H
#define LOCATION_H

#include "HyperionLibrary_global.h"

#include <QJsonObject>
#include "mathconstants.h"

#include <Eigen/Dense>

namespace hypl
{
    class HYPERIONLIBRARY_EXPORT Location
    {
    public:
        Location() : m_latitude {37.2 * mathconstants::degree} {} // Latitude stored in radians      0.0174533
        Location(double latitude) : m_latitude {latitude} {} //Latitude should be entered in radians

        //Accessors
        double latitude() const { return m_latitude; }

        //Mutators
        void set_latitude_degree(double latitude_degree) { m_latitude = latitude_degree * mathconstants::degree; }

        //Public functions
        double LatitudeDegree() const { return m_latitude / mathconstants::degree; }
        double HourAngleLimit(double declination) const;
        Eigen::Vector3d SolarVector(double hour_angle, double declination) const;
        void Read(const QJsonObject& json);
        void Write(QJsonObject& json) const;

    private:
        double m_latitude;
    };
}
#endif // LOCATION_H
