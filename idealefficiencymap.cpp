#include <vector>
#include <functional>
#include <algorithm>
#include <execution>

#include "auxfunction.h"

#include "idealefficiencymap.h"

void heliostatAnnualIdealEfficiency(hypl::Heliostat* pHeliostat);

hypl::IdealEfficiencyMap::IdealEfficiencyMap(Environment environment, Boundaries boundaries, 
                                                std::vector<Receiver> receivers, int nrows, int ncolumns)
    : m_environment {environment},
      m_boundaries {boundaries},
      m_receivers {receivers},
      m_nrows {nrows},
      m_ncolumns {ncolumns}
{
    regenerate();
}

void hypl::IdealEfficiencyMap::regenerate()
{
    if( m_heliostats.empty() == false ) 
    {
        for (auto& element : m_heliostats) delete element;
        m_heliostats.clear();
    }

    double dx = (m_boundaries.xmax() - m_boundaries.xmin())/m_ncolumns;
    double dy = (m_boundaries.ymax() - m_boundaries.ymin())/m_nrows;

    for(int row=0; row < m_nrows; row++ )
    {
        double y = m_boundaries.ymin() + dy * (0.5 + row);
        for(int column=0; column < m_ncolumns; column++)
        {
            double x = m_boundaries.xmin() + dx * (0.5 + column);
            hypl::vec3d heliostat_center(x, y, 0.0);
            m_heliostats.push_back(new hypl::Heliostat(m_environment, m_receivers, heliostat_center));
        }
    }
    m_heliostats.shrink_to_fit();
}

void hypl::IdealEfficiencyMap::update()
{
    for (auto & element : m_heliostats) element->update ();
}


void hypl::IdealEfficiencyMap::EvaluateAnnualEfficiencies()
{
    std::for_each(std::execution::par_unseq, m_heliostats.begin(), m_heliostats.end(), heliostatAnnualIdealEfficiency);
}

hypl::IdealEfficiencyMap::~IdealEfficiencyMap()
{
     for (auto& element : m_heliostats) delete element;
}

void heliostatAnnualIdealEfficiency(hypl::Heliostat* pHeliostat)
{
    pHeliostat->annual_ideal_efficiency();
}