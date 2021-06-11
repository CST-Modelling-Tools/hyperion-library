#ifndef HELIOSTATPOWERPERUNITAREA_H
#define HELIOSTATPOWERPERUNITAREA_H

#include "export.h"

#include "environment.h"
#include "heliostat.h"

namespace hypl
{
    class DLL_API HeliostatPowerPerUnitArea
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
