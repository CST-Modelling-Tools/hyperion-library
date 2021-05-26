#include <QFile>
#include <QVector>

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

#include "auxfunction.h"

#include "heliostatfield.h"

hypl::HeliostatField::HeliostatField(Location location, Atmosphere atmosphere, Boundaries boundaries,
                                     int nrows, int ncolumns, QVector<Receiver> receivers)
    : m_location {location},
      m_atmosphere {atmosphere},
      m_boundaries {boundaries},
      m_nrows {nrows},
      m_ncolumns {ncolumns},
      m_receivers {receivers}
{
    regenerate();
}

void hypl::HeliostatField::regenerate()
{
    if( m_heliostats.isEmpty() == false )
    {
        for (auto& element : m_heliostats) delete element;
         m_heliostats.clear();
    }

    if (m_nrows >0 && m_ncolumns > 0)
    {
        double dx = (m_boundaries.xmax() - m_boundaries.xmin())/m_ncolumns;
        double dy = (m_boundaries.ymax() - m_boundaries.ymin())/m_nrows;

        for(int row=0; row < m_nrows; row++ )
        {
            double y = m_boundaries.ymin() + dy * (0.5 + row);
            for(int column=0; column < m_ncolumns; column++)
            {
                double x = m_boundaries.xmin() + dx * (0.5 + column);
                Eigen::Vector3d heliostat_center(x, y, 0.0);
                m_heliostats.append(new Heliostat(m_location, m_atmosphere, m_receivers, heliostat_center));
            }
        }
    }
    m_heliostats.squeeze();
}

void hypl::HeliostatField::update()
{
    for (auto & element : m_heliostats) element->update ();
}

QVector<double> hypl::HeliostatField::AnnualIdealEfficiencies()
{
    QVector<double> annual_ideal_efficiency_vector;

    double delta_days = 26;
    double dni_yearly_energy = auxfunction::DniYearlyEnergy(m_location, m_atmosphere, delta_days);

    for (auto& element : m_heliostats)
    {
        double annual_ideal_efficiency = element->HeliostatAnnualIdealEfficency(delta_days, dni_yearly_energy);
        annual_ideal_efficiency_vector.append(annual_ideal_efficiency);
    }
    return annual_ideal_efficiency_vector;
}

void hypl::HeliostatField::Read(const QJsonObject& json)
{
    m_location.Read(json["Location"].toObject());
    m_atmosphere.Read(json["Atmosphere"].toObject());
    m_boundaries.Read(json["Boundaries"].toObject());
    m_nrows = json["nrows"].toInt();
    m_ncolumns = json["ncolumns"].toInt();

    if( m_receivers.isEmpty() == false ) m_receivers.clear();

    //Read the receivers array from the JSON file
    QJsonArray receivers_array = json["Receivers"].toArray();
    for (int receivers_index = 0; receivers_index < receivers_array.size(); ++receivers_index)
    {
        QJsonObject receiver_object = receivers_array[receivers_index].toObject();
        Receiver receiver;
        receiver.Read(receiver_object);
        m_receivers.append(receiver);
    }
}

void hypl::HeliostatField::Write(QJsonObject& json) const
{
    QJsonObject location_object;
    m_location.Write(location_object);
    json["Location"] = location_object;

    QJsonObject atmosphere_object;
    m_atmosphere.Write(atmosphere_object);
    json["Atmosphere"] = atmosphere_object;

    QJsonObject boundary_object;
    m_boundaries.Write(boundary_object);
    json["Boundary"] = boundary_object;

    json["nrows"] = m_nrows;
    json["ncolumns"] = m_ncolumns;

    QJsonArray receivers_array;
    for (auto& element : m_receivers)
    {
        QJsonObject receiver_object;
        element.Write(receiver_object);
        receivers_array.append(receiver_object);
    }
    json["Receivers"] = receivers_array;
}

bool hypl::HeliostatField::LoadHeliostatField(QString filename, SaveFormat save_format)
{
    QFile loadFile(save_format == Json
                   ? filename.append(".json")
                   : filename.append(".dat"));

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray save_data = loadFile.readAll();

    QJsonDocument loadDoc(save_format == Json
            ? QJsonDocument::fromJson(save_data)
            : QJsonDocument::fromBinaryData(save_data));

    Read(loadDoc.object());

    return true;
}

bool hypl::HeliostatField::SaveHeliostatField(QString filename, SaveFormat save_format) const
{
    QFile save_file(save_format == Json
        ? filename.append(".json")
        : filename.append(".dat"));


    if (!save_file.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject simulation_object;
    Write(simulation_object);

    QJsonDocument save_doc(simulation_object);
    save_file.write(save_format == Json
        ? save_doc.toJson()
        : save_doc.toBinaryData());

    return true;
}

hypl::HeliostatField::~HeliostatField()
{
    if( m_heliostats.isEmpty() == false )
    {
        for (auto& element : m_heliostats) delete element;
        m_heliostats.clear();
    }
}
