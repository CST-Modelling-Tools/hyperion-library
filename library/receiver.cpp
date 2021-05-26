#include <QDebug>
#include "receiver.h"

void hypl::Receiver::Read(const QJsonObject& json)
{
    double x = json["x"].toDouble();
    double y = json["y"].toDouble();
    double z = json["z"].toDouble();
    m_aiming_point(0) = x;
    m_aiming_point(1) = y;
    m_aiming_point(2) = z;
}

void hypl::Receiver::Write(QJsonObject& json) const
{
    json["x"] = m_aiming_point(0);
    json["y"] = m_aiming_point(1);
    json["z"] = m_aiming_point(2);
}


