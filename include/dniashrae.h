#ifndef DNIASHRAE_H
#define DNIASHRAE_H

#include "export.h"
#include "environment.h"

namespace hypl
{
    class DLL_API DniASHRAE
    {
    public:
        DniASHRAE(double declination, const Environment& environment) : 
            m_declination {declination},
            m_environment {environment} { }
        double operator()(double solar_time) const;

        //Accessors
        double declination() const { return m_declination; }
        const Environment& environment() const { return m_environment; }       

    private:
        double m_declination;
        const Environment& m_environment;
    };
}
#endif // DNIASHRAE_H
