#ifndef ATTENUATION_H
#define ATTENUATION_H

#include "export.h"

namespace hypl
{
    class DLL_API Atmosphere
    {
    public:

        enum AttenuationModel { SW, VB, LH };

        Atmosphere() :
            m_io {1110.0},
            m_beta {0.11},
            m_attenuation_model {AttenuationModel::VB} {}

        Atmosphere(double io, double beta, Atmosphere::AttenuationModel attenuation_model)
            : m_io {io}, 
              m_beta {beta}, 
              m_attenuation_model {attenuation_model} {}

        //Accessors
        double io() const { return m_io; }
        double beta() const {return m_beta; }
        AttenuationModel attenuation_model() const { return m_attenuation_model; }

        //Mutators
        void set_io(double io) { m_io = io; }
        void set_beta( double beta) { m_beta = beta; }
        void set_attenuation_model(AttenuationModel attenuation_model) {m_attenuation_model = attenuation_model; }

        //Specific public functions
        double Attenuation(double slant_range ) const;
        double DniFromSz(double sz) const;

    private:
        //Private functions
        double AttenuationSW(double slant_range) const;
        double AttenuationVB(double slant_range) const;
        double AttenuationLH(double slant_range) const;

        //Private variables
        double m_io;
        double m_beta;
        AttenuationModel m_attenuation_model;
    };
}

#endif // ATTENUATION_H
