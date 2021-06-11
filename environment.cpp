#include "environment.h"
#include "auxfunction.h"

hypl::Environment::Environment( Location location, Atmosphere atmosphere, int delta_days) :
            m_location {location},
            m_atmosphere {atmosphere},
            m_delta_days {delta_days} { m_dni_annual_energy = auxfunction::DniYearlyEnergy( (*this) ); }

hypl::Environment::Environment() :
            m_delta_days {13}
            { m_dni_annual_energy = auxfunction::DniYearlyEnergy( (*this) ); }

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