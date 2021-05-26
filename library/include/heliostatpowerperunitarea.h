#ifndef HELIOSTATPOWERPERUNITAREA_H
#define HELIOSTATPOWERPERUNITAREA_H

#include "HyperionLibrary_global.h"

#include "location.h"
#include "atmosphere.h"
#include "heliostat.h"

namespace hypl
{
    class HYPERIONLIBRARY_EXPORT HeliostatPowerPerUnitArea
    {
    public:
        HeliostatPowerPerUnitArea(const Heliostat& heliostat, double declination);
        double operator()(double solar_time) const;

    private:
        const Heliostat& m_heliostat;
        double m_declination;
    };
}
#endif // HELIOSTATPOWERPERUNITAREA_H
