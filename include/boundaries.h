#ifndef BOUNDARIES_H
#define BOUNDARIES_H

#include "export.h"

namespace hypl
{
    class DLL_API Boundaries
    {
    public:
        static constexpr double default_xmin = -500.0;
        static constexpr double default_xmax = 500.0;
        static constexpr double default_ymin = -100.0;
        static constexpr double default_ymax = 700.0;

        Boundaries() :
            m_xmin{default_xmin}, 
            m_xmax{default_xmax}, 
            m_ymin{default_ymin}, 
            m_ymax{default_ymax} {}

    Boundaries(double xmin, double xmax, double ymin, double ymax) :
        m_xmin{(xmin <= xmax) ? xmin : xmax},
        m_xmax{(xmin <= xmax) ? xmax : xmin},
        m_ymin{(ymin <= ymax) ? ymin : ymax},
        m_ymax{(ymin <= ymax) ? ymax : ymin} {}


        //Accessors
        double const& xmin() const {return m_xmin;}
        double const& xmax() const {return m_xmax;}
        double const& ymin() const {return m_ymin;}
        double const& ymax() const {return m_ymax;}

        // Mutators with validation
        void set_xmin(double xmin) { if (xmin <= m_xmax) m_xmin = xmin; }
        void set_xmax(double xmax) { if (xmax >= m_xmin) m_xmax = xmax; }
        void set_ymin(double ymin) { if (ymin <= m_ymax) m_ymin = ymin; }
        void set_ymax(double ymax) { if (ymax >= m_ymin) m_ymax = ymax; }

        // Utility functions
        double width() const { return m_xmax - m_xmin; }
        double height() const { return m_ymax - m_ymin; }        

    private:
        double m_xmin;
        double m_xmax;
        double m_ymin;
        double m_ymax;
    };
}
#endif // BOUNDARIES_H