#ifndef HELIOSTATFIELD_H
#define HELIOSTATFIELD_H

#include "HyperionLibrary_global.h"

#include <QVector>

#include "location.h"
#include "atmosphere.h"
#include "boundaries.h"
#include "receiver.h"
#include "heliostat.h"

namespace hypl
{
    class HYPERIONLIBRARY_EXPORT HeliostatField
    {
    public:
        enum SaveFormat { Json, Binary };

        HeliostatField() {}
        HeliostatField(Location location, Atmosphere atmosphere, Boundaries boundaries, int nrows, int ncolumns, QVector<Receiver> m_receivers);
        ~HeliostatField();

        //Accessors
        Location location() const {return m_location;}
        Atmosphere atmosphere() const {return m_atmosphere;}
        Boundaries boundaries() const {return m_boundaries;}
        int nrows() const {return m_nrows;}
        int ncolumns() const {return m_ncolumns;}
        QVector<Receiver> receivers() const {return m_receivers;}
        QVector<Heliostat*> heliostats() const {return m_heliostats;}

        //Mutators
        void update();
        void regenerate();
        void set_location(Location location) {m_location = location;}
        void set_atmosphere(Atmosphere atmosphere) {m_atmosphere = atmosphere; regenerate();}
        void set_boundaries(Boundaries boundaries) {m_boundaries = boundaries; regenerate();}
        void set_nrows(int nrows) {m_nrows = nrows; regenerate();}
        void set_ncolumns(int ncolumns) {m_ncolumns = ncolumns; regenerate();}
        void set_receivers(QVector<Receiver> receivers) {m_receivers = receivers; regenerate();}

        //Public functions
        QVector<double> AnnualIdealEfficiencies();

        bool LoadHeliostatField(QString filename, SaveFormat save_format);
        bool SaveHeliostatField(QString filename, SaveFormat save_format) const;
        void Read(const QJsonObject& json);
        void Write(QJsonObject& json) const;

    private:
        Location m_location;
        Atmosphere m_atmosphere;
        Boundaries m_boundaries;
        int m_nrows;
        int m_ncolumns;
        QVector<Receiver> m_receivers;
        QVector<Heliostat*> m_heliostats;
    };
}
#endif // HELIOSTATFIELD_H
