#include <QDebug>
#include "boundaries.h"

void hypl::Boundaries::Read(const QJsonObject &json)
{
    m_xmin = json["xmin"].toDouble();
    m_xmax = json["xmax"].toDouble();
    m_ymin = json["ymin"].toDouble();
    m_ymax = json["ymax"].toDouble();
}

void hypl::Boundaries::Write(QJsonObject &json) const
{
    json["xmin"] = m_xmin;
    json["xmax"] = m_xmax;
    json["ymin"] = m_ymin;
    json["ymax"] = m_ymax;
}
