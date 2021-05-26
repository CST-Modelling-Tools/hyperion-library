#ifndef BOUNDARIES_H
#define BOUNDARIES_H

#include "HyperionLibrary_global.h"

#include <QJsonObject>

namespace hypl
{
    class HYPERIONLIBRARY_EXPORT Boundaries
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
        double xmin() const {return m_xmin;}
        double xmax() const {return m_xmax;}
        double ymin() const {return m_ymin;}
        double ymax() const {return m_ymax;}

        //Mutators
        void set_xmin(double xmin) {m_xmin = xmin;}
        void set_xmax(double xmax) {m_xmax = xmax;}
        void set_ymin(double ymin) {m_ymin = ymin;}
        void set_ymax(double ymax) {m_ymax = ymax;}

        //Common public functions
        void Read(const QJsonObject &json);
        void Write(QJsonObject &json) const;

    private:
        double m_xmin;
        double m_xmax;
        double m_ymin;
        double m_ymax;
    };
}
#endif // BOUNDARIES_H
