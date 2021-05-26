#ifndef DNIASHRAE_H
#define DNIASHRAE_H

#include "HyperionLibrary_global.h"

#include "location.h"
#include "atmosphere.h"

namespace hypl
{
    class HYPERIONLIBRARY_EXPORT DniASHRAE
    {
    public:
        DniASHRAE(double declination, const Location& location, const Atmosphere& atmosphere);
        double operator()(double solar_time) const;

    private:
        double m_declination;
        const Location& m_location;
        const Atmosphere& m_atmosphere;
    };
}
#endif // DNIASHRAE_H
