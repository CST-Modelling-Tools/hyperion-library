#include <functional>
#include <algorithm>
#include <execution>

#include "idealefficiencymap.h"
#include "auxfunction.h"
#include "processheliostatfunctor.h"


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

void hypl::IdealEfficiencyMap::EvaluateAnnualEfficiencies(Heliostat::IdealEfficiencyType ideal_efficiency_type, double delta_t)
{
    double delta_hour_angle = delta_t * hypl::mathconstants::earth_rotational_speed;

    // Initialization of key variables
    double direct_insolation = 0.0;
    for (auto& element : m_heliostat) element.m_annual_ideal_efficiency = 0.0;

    // Computation for half of the year but multiplying the dni by 2.0

    double declination;
    double sun_subtended_angle;
    double wo;
    double hour_angle;
    int day_number;

    double weight = 2.0;
    for(day_number=174; day_number<356; day_number++)
    {
        ProcessDay(day_number, ideal_efficiency_type, delta_hour_angle, weight, direct_insolation);
    }
 

    weight = 0.5;
    day_number = 173;
    ProcessDay(day_number, ideal_efficiency_type, delta_hour_angle, weight, direct_insolation);

    for (auto& element : m_heliostat) element.m_annual_ideal_efficiency = element.m_annual_ideal_efficiency/direct_insolation;
}

void hypl::IdealEfficiencyMap::ProcessDay(int day_number, Heliostat::IdealEfficiencyType ideal_efficiency_type, 
                                          double delta_hour_angle, double weight, double& direct_insolation)
{
    double declination = auxfunction::SolarDeclinationByDayNumber(day_number);
    double sun_subtended_angle = m_environment.sun_subtended_angle()[day_number-1];
    double wo = m_environment.location().HourAngleLimit(declination);
    double hour_angle = StartingHourAngle(wo, delta_hour_angle);
    while (hour_angle < wo)
    {
        vec3d sun_vector = m_environment.location().SolarVector(hour_angle, declination);
        double dni = weight * m_environment.atmosphere().DniFromSz(sun_vector.z);
        ProcessHeliostatFunctor process_heliostat(sun_vector, sun_subtended_angle, ideal_efficiency_type, dni);
        
        direct_insolation += dni;
        std::for_each(std::execution::par_unseq, m_heliostat.begin(), m_heliostat.end(), process_heliostat);
        hour_angle += delta_hour_angle;
    }
}

double hypl::IdealEfficiencyMap::StartingHourAngle(double wo, double delta_hour_angle)
{
       double p, fraction;
       fraction = std::modf( (2.0*wo)/delta_hour_angle, &p );
       return -wo + ((fraction * delta_hour_angle)/2.0); 
}