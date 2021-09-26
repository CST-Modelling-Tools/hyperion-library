#include "environment.h"
#include "auxfunction.h"

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
    for (int day_number=1; day_number<=ndays; day_number++)
    {
        m_sun_subtended_angle.emplace_back(auxfunction::ReceiverSubtendedAngle(mathconstants::sun_radius / auxfunction::DistanceSunEarth(day_number)));
        m_declination.emplace_back(auxfunction::SolarDeclinationByDayNumber(day_number));
    }
}
