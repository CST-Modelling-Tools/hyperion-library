#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H

#include "export.h"

namespace hypl
{
    class DLL_API Atmosphere
    {
    public:

        enum TransmittanceModel { SW, VB, LH };

        Atmosphere() :
            m_io {1110.0},
            m_beta {0.11},
            m_transmittance_model {TransmittanceModel::VB} {}

        Atmosphere(double io, double beta, Atmosphere::TransmittanceModel transmittance_model)
            : m_io {io}, 
              m_beta {beta}, 
              m_transmittance_model {transmittance_model} {}

        //Accessors
        double io() const { return m_io; }
        double beta() const {return m_beta; }
        TransmittanceModel transmittance_model() const { return m_transmittance_model; }

        //Mutators
        void set_io(double io) { m_io = io; }
        void set_beta( double beta) { m_beta = beta; }
        void set_transmittance_model(TransmittanceModel transmittance_model) {m_transmittance_model = transmittance_model; }

        //Specific public functions
        double Transmittance(double slant_range ) const;
        double DniFromSz(double sz) const;

    private:
        //Private functions
        double TransmittanceSW(double slant_range) const;
        double TransmittanceVB(double slant_range) const;
        double TransmittanceLH(double slant_range) const;

        //Private variables
        double m_io;
        double m_beta;
        TransmittanceModel m_transmittance_model;
    };
}

#endif // ATMOSPHERE_H