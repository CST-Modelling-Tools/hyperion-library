#ifndef PROCESSHELISTATFUNCTOR_H
#define PROCESSHELISTATFUNCTOR_H

#include "export.h"

#include "environment.h"
#include "heliostat.h"

namespace hypl
{
    class DLL_API ProcessHeliostatFunctor
    {
    public:
        ProcessHeliostatFunctor(vec3d& sun_vector, double sun_subtended_angle, hypl::Heliostat::IdealEfficiencyType ideal_efficiency_type, double dni);
        void operator()(Heliostat& heliostat) const;

    private:
        vec3d& m_sun_vector;
        double m_sun_subtended_angle;
        hypl::Heliostat::IdealEfficiencyType m_ideal_efficiency_type;
        double m_dni;
    };
}
#endif // PROCESSHELISTATFUNCTOR_H