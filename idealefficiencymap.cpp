#include <vector>
#include <functional>
#include <algorithm>
#include <execution>

#include "auxfunction.h"
#include "idealefficiencymap.h"

hypl::IdealEfficiencyMap::IdealEfficiencyMap(Environment environment, Boundaries boundaries, 
                                            std::vector<Receiver>& receivers, int nrows, int ncolumns) :
m_environment {environment},
m_boundaries {boundaries},
m_receiver {receivers},
m_nrows {nrows},
m_ncolumns {ncolumns}
{
    regenerate();
}

void hypl::IdealEfficiencyMap::regenerate()
{
    if( m_heliostat.empty() == false ) m_heliostat.clear();

    double dx = (m_boundaries.xmax() - m_boundaries.xmin())/m_ncolumns;
    double dy = (m_boundaries.ymax() - m_boundaries.ymin())/m_nrows;

    for(int row=0; row < m_nrows; row++ )
    {
        double y = m_boundaries.ymin() + dy * (0.5 + row);
        for(int column=0; column < m_ncolumns; column++)
        {
            double x = m_boundaries.xmin() + dx * (0.5 + column);
            hypl::vec3d heliostat_center(x, y, 0.0);
            m_heliostat.emplace_back(m_environment, m_receiver, heliostat_center);
        }
    }
    m_heliostat.shrink_to_fit();
}

void hypl::IdealEfficiencyMap::update()
{
    for (auto & element : m_heliostat) element.update ();
}

void hypl::IdealEfficiencyMap::set_receivers_radius(double radius)
{
    for (auto & element : m_receiver) element.set_radius (radius);
}

void hypl::IdealEfficiencyMap::EvaluateAnnualEfficiencies(hypl::Heliostat::IdealEfficiencyType ideal_efficiency_type, double delta_t)
{
    double direct_insolation = 0.0;
    for (int heliostat_id = 0; heliostat_id < m_heliostat.size(); heliostat_id++)
    {
         m_heliostat[heliostat_id].m_annual_ideal_efficiency = 0.0;
    }


    int day_number = 1;
    while (day_number < 366)
    {
        double declination = auxfunction::SolarDeclinationByDayNumber(day_number);
        double sun_subtended_angle = m_environment.sun_subtended_angle()[day_number-1];
        double wo = m_environment.location().HourAngleLimit(declination);
        double t_start = -wo/mathconstants::earth_rotational_speed; // solar time in seconds.
        double t_end = -t_start;
        double t = t_start;
        while (t < t_end)
        {
            double hour_angle = t * mathconstants::earth_rotational_speed;
            vec3d sun_vector = m_environment.location().SolarVector(hour_angle, declination);
            double dni = m_environment.atmosphere().DniFromSz(sun_vector.z);
            direct_insolation += dni;
            for (int heliostat_id = 0; heliostat_id < m_heliostat.size(); heliostat_id++)
            {
                 m_heliostat[heliostat_id].m_annual_ideal_efficiency += m_heliostat[heliostat_id].Track(sun_vector, sun_subtended_angle, ideal_efficiency_type).ideal_efficiency * dni;
            }
            t += delta_t;
        }
        day_number++;
    }
    
    for (int heliostat_id = 0; heliostat_id < m_heliostat.size(); heliostat_id++)
    {
         m_heliostat[heliostat_id].m_annual_ideal_efficiency =  m_heliostat[heliostat_id].m_annual_ideal_efficiency/direct_insolation;
    }
}
