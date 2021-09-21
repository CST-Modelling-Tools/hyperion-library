#include "environment.h"
#include "auxfunction.h"
#include "gcf.h"

hypl::Environment::Environment( Location location, Atmosphere atmosphere, int delta_days) :
            m_location {location},
            m_atmosphere {atmosphere},
            m_delta_days {delta_days}
            {
                update_environment();
            }

hypl::Environment::Environment() :
            m_delta_days {13}
            {
                update_environment();
            }

void hypl::Environment::update_environment()
{
    m_dni_annual_energy = auxfunction::DniYearlyEnergy( (*this) );

    if( m_sun_subtended_angles.empty() == false ) m_declination.clear();
    if( m_declination.empty() == false ) m_declination.clear();
    int ndays = 365;
    m_sun_subtended_angles.reserve(ndays);
    m_declination.reserve(ndays);
    for (int i=1; i<=ndays; i++)
    {
        m_sun_subtended_angles.emplace_back(auxfunction::Subtended_angle(gcf::Sun_radius / auxfunction::Distance_earth_sun(i)));
        m_declination.emplace_back(auxfunction::SolarDeclinationByDayNumber(i));
    }
}

void hypl::Environment::set_location(Location location) 
{
    m_location = location; m_dni_annual_energy = auxfunction::DniYearlyEnergy( (*this) ); 
}

void hypl::Environment::set_atmosphere(Atmosphere atmosphere) 
{ 
    m_atmosphere = atmosphere; m_dni_annual_energy = auxfunction::DniYearlyEnergy( (*this) ); 
}

void hypl::Environment::set_delta_days(int delta_days)
{
    m_delta_days = delta_days;
    m_dni_annual_energy = auxfunction::DniYearlyEnergy((*this));
}

void hypl::Environment::set_location_and_atmosphere(Location location, Atmosphere atmosphere)
{
    m_location = location;
    m_atmosphere = atmosphere;
    m_dni_annual_energy = auxfunction::DniYearlyEnergy( (*this) );
}

const double hypl::Environment::get_sun_subtended_angle(int day_number) const
{
    if(day_number<m_sun_subtended_angles.size()) return m_sun_subtended_angles[day_number-1];
    else return m_sun_subtended_angles[day_number-1-m_sun_subtended_angles.size()];
}