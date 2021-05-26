#include <QDebug>
#include "location.h"
#include "mathconstants.h"

double hypl::Location::HourAngleLimit(double declination) const
{
    return abs(acos(-tan(declination)*tan(m_latitude)));
}

Eigen::Vector3d hypl::Location::SolarVector(double hour_angle, double declination) const
{
    double cos_declination = cos(declination);
    double sin_declination = sin(declination);
    double cos_latitude = cos(m_latitude);
    double sin_latitude = sin(m_latitude);
    double cos_hour_angle = cos(hour_angle);

    return Eigen::Vector3d(
             -cos_declination * sin(hour_angle),
             sin_declination * cos_latitude - cos_declination * sin_latitude * cos_hour_angle,
             sin_declination * sin_latitude + cos_declination * cos_latitude * cos_hour_angle );
}


void hypl::Location::Read(const QJsonObject& json)
{
    m_latitude = json["Latitude"].toDouble() * mathconstants::degree; //Storing latitute in radians
}


void hypl::Location::Write(QJsonObject& json) const
{
    json["Latitude"] = LatitudeDegree(); //Saving in the file the latitute in degrees and not in radians
}

