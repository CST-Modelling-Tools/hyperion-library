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
        Environment( Location location, Atmosphere atmosphere, int delta_days);


        //Accessors
        const Location& location() const { return m_location; }
        const Atmosphere& atmosphere() const {return m_atmosphere; }
        const int& delta_days() const { return m_delta_days; }
        double dni_annual_energy() const { return m_dni_annual_energy; }
        const std::vector<double>& sun_subtended_angles() const { return m_sun_subtended_angles; }
        const double get_sun_subtended_angle(int day_number) const;
        const double get_declination(int day_number) const { return m_declination[day_number-1]; } 

        //Mutators
        void set_location(Location location);
        void set_atmosphere(Atmosphere atmosphere);
        void set_delta_days(int delta_days);
        void set_location_and_atmosphere(Location location, Atmosphere atmosphere);
        void update_environment();

    private:
        //Private variables
        Location m_location;
        Atmosphere m_atmosphere;
        int m_delta_days;
        double m_dni_annual_energy;
        std::vector<double> m_sun_subtended_angles;
        std::vector<double> m_declination;
    };
}

#endif // ENVIRONMENT_H