#ifndef LOCATION_H
#define LOCATION_H

#include "export.h"

#include "mathconstants.h"
#include "vec3d.h"

namespace hypl
{
    class DLL_API Location
    {
    public:
        Location() : m_latitude {37.2 * mathconstants::degree} {} // Latitude stored in radians 0.0174533
        Location(double latitude) : m_latitude {latitude} {} //Latitude should be entered in radians

        //Accessors
        double latitude() const { return m_latitude; }

        //Mutators
        void set_latitude_degree(double latitude_degree) { m_latitude = latitude_degree * mathconstants::degree; }

        //Public functions
        double LatitudeDegree() const { return m_latitude / mathconstants::degree; }
        double HourAngleLimit(double declination) const;
        vec3d SolarVector(double hour_angle, double declination) const;

    private:
        double m_latitude;
    };
}
#endif // LOCATION_H
