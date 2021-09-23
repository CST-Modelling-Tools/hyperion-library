#include <cmath>
#include "mathconstants.h"
#include "auxfunction.h"
#include "heliostatinstantaneousefficiency.h"

hypl::HeliostatInstantaneousEfficiency::HeliostatInstantaneousEfficiency(vec3d& sun_vector, 
                                            double sun_subtended_angle, hypl::Heliostat::IdealEfficiencyType ideal_efficiency_type, double dni) :
m_sun_vector {sun_vector},
m_sun_subtended_angle {sun_subtended_angle},
m_ideal_efficiency_type {ideal_efficiency_type},
m_dni {dni}
{
}

void hypl::HeliostatInstantaneousEfficiency::operator()(Heliostat& heliostat) const
{
    heliostat.m_annual_ideal_efficiency += heliostat.Track(m_sun_vector, m_sun_subtended_angle, m_ideal_efficiency_type).ideal_efficiency * m_dni;
}
