#include <math.h>
#include "atmosphere.h"

double hypl::Atmosphere::Attenuation(double slant_range) const
{
    switch ( m_attenuation_model )
    {
        case AttenuationModel::SW:
            return AttenuationSW(slant_range);
            break;
        case AttenuationModel::VB:
            return AttenuationVB(slant_range);
            break;
        case AttenuationModel::LH:
            return AttenuationLH(slant_range);
            break;
        default:
            return 1.0;
            break;
    }
}

double hypl::Atmosphere::AttenuationSW(double slant_range) const
{
    return exp(-(0.000010696 + 0.0009196 * m_beta) * slant_range);
}

double hypl::Atmosphere::AttenuationVB(double slant_range) const
{
    return 0.99326 + slant_range * (-0.0001046 + (1.7000000000000003e-8 - 2.845e-12 * slant_range) * slant_range);
}

double hypl::Atmosphere::AttenuationLH(double slant_range) const
{
    if(slant_range <= 1000.0) return 0.99321 + (-0.0001176 + 1.9699999999999998e-8 * slant_range) * slant_range;
    else return exp(-0.0001106 * slant_range);
}

double hypl::Atmosphere::DniFromSz(double sz) const
{
    if(sz>0.00001) return m_io * exp(-m_beta/sz);
    else return 0.0;
}