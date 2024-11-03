#ifndef BOUNDARIES_H
#define BOUNDARIES_H

#include "export.h"

namespace hypl
{
    class DLL_API Boundaries
    {
    public:
        Boundaries() :
            m_xmin {-500.},
            m_xmax {500.},
            m_ymin {-100.},
            m_ymax {700.} {}

        Boundaries(double xmin, double xmax, double ymin, double ymax)
            : m_xmin {xmin},
              m_xmax {xmax},
              m_ymin {ymin},
              m_ymax {ymax} {}

        //Accessors
        double const& xmin() const {return m_xmin;}
        double const& xmax() const {return m_xmax;}
        double const& ymin() const {return m_ymin;}
        double const& ymax() const {return m_ymax;}

        //Mutators
        void set_xmin(double xmin) {m_xmin = xmin;}
        void set_xmax(double xmax) {m_xmax = xmax;}
        void set_ymin(double ymin) {m_ymin = ymin;}
        void set_ymax(double ymax) {m_ymax = ymax;}

    private:
        double m_xmin;
        double m_xmax;
        double m_ymin;
        double m_ymax;
    };
}
#endif // BOUNDARIES_H
