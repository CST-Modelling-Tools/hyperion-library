#ifndef LOCATION_H
#define LOCATION_H

#include "export.h"

#include "mathconstants.h"
#include "vec3d.h"

namespace hypl
{
    class HYPERIONLIBRARY_API Location
    {
    public:
        Location() : m_latitude {37.2 * mathconstants::degree},
            m_cos_latitude {cos(m_latitude)},
            m_sin_latitude {sin(m_latitude)}
            {} // Latitude stored in radians 0.0174533

        Location(double latitude) : m_latitude {latitude},
            m_cos_latitude {cos(m_latitude)},
            m_sin_latitude {sin(m_latitude)}        
            {} //Latitude should be entered in radians

        //Accessors
        double const& latitude() const { return m_latitude; }
        double const& cos_latitude() const { return m_cos_latitude; }
        double const& sin_latitude() const { return m_sin_latitude; }

        //Mutators
        void set_latitude_degree(double latitude_degree) { m_latitude = latitude_degree * mathconstants::degree; 
                                                           m_cos_latitude = cos(m_latitude); 
                                                           m_sin_latitude = sin(m_latitude); }

        //Public functions
        double LatitudeDegree() const { return m_latitude / mathconstants::degree; }
        double HourAngleLimit(double declination) const;
        vec3d SolarVector(double hour_angle, double declination) const;

    private:
        double m_latitude;
        double m_cos_latitude;
        double m_sin_latitude;
    };
}
#endif // LOCATION_H
