#include "environment.h"
#include "auxfunction.h"
#include "gcf.h"

hypl::Environment::Environment()
{
    update_environment();
}

hypl::Environment::Environment( Location location, Atmosphere atmosphere) :
m_location {location},
m_atmosphere {atmosphere}
{
    update_environment();
}

void hypl::Environment::update_environment()
{

    if( m_sun_subtended_angle.empty() == false ) m_declination.clear();
    if( m_declination.empty() == false ) m_declination.clear();

    // ACTION - Modify to lines below take into account the actual days of the year we are using to estimate things 173 to 355??
    int ndays = 365;  
    m_sun_subtended_angle.reserve(ndays);
    m_declination.reserve(ndays);
    for (int i=1; i<=ndays; i++)
    {
        m_sun_subtended_angle.emplace_back(auxfunction::ReceiverSubtendedAngle(gcf::Sun_radius / auxfunction::DistanceSunEarth(i)));
        m_declination.emplace_back(auxfunction::SolarDeclinationByDayNumber(i));
    }
}

void hypl::Environment::set_location(Location location) 
{
    m_location = location; 
}

void hypl::Environment::set_atmosphere(Atmosphere atmosphere) 
{ 
    m_atmosphere = atmosphere; 
}

void hypl::Environment::set_location_and_atmosphere(Location location, Atmosphere atmosphere)
{
    m_location = location;
    m_atmosphere = atmosphere;
}
