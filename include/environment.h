#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>

#include "export.h"
#include "location.h"
#include "atmosphere.h"

namespace hypl
{
    class DLL_API Environment
    {
    public:

        Environment();
        Environment(Location location, Atmosphere atmosphere);


        //Accessors
        Location const& location() const { return m_location; }
        Atmosphere const& atmosphere() const { return m_atmosphere; }
        std::vector<double> const& sun_subtended_angle() const { return m_sun_subtended_angle; }
        std::vector<double> const& declination() const { return m_declination; }

        //Mutators
        void set_location(Location location) { m_location = location; }
        void set_atmosphere(Atmosphere atmosphere) { m_atmosphere = atmosphere; }
        void set_location_and_atmosphere(Location location, Atmosphere atmosphere) { m_location = location; m_atmosphere = atmosphere; }
        void update_environment();

    private:
        //Private variables
        Location m_location;
        Atmosphere m_atmosphere;
        std::vector<double> m_sun_subtended_angle;
        std::vector<double> m_declination;
    };
}

#endif // ENVIRONMENT_H