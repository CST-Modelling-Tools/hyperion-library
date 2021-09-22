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
        const Location& location() const { return m_location; }
        const Atmosphere& atmosphere() const { return m_atmosphere; }
        double dni_annual_energy() const { return m_dni_annual_energy; }
        const std::vector<double>& sun_subtended_angle() const { return m_sun_subtended_angle; }
        const std::vector<double>& declination() const { return m_declination; }

        //Mutators
        void set_location(Location location);
        void set_atmosphere(Atmosphere atmosphere);
        void set_location_and_atmosphere(Location location, Atmosphere atmosphere);
        void update_environment();

    private:
        //Private variables
        Location m_location;
        Atmosphere m_atmosphere;
        double m_dni_annual_energy;
        std::vector<double> m_sun_subtended_angle;
        std::vector<double> m_declination;
    };
}

#endif // ENVIRONMENT_H