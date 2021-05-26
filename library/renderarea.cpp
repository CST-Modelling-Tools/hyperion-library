#include <QPainter>
#include "heliostatrectangular.h"
#include "renderarea.h"
#include <QDebug>


RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    m_pBuffer = new QPixmap(1000,1000);
    m_pBuffer->fill(Qt::red);

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(1000,1000);
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    QVector3D solar_vector(0.0, 0.0, 1.0);
    QVector3D heliostat_center(0.0, 0.0, 0.0);
    QVector3D heliostat_normal(0.0, 0.0, 1.0);
    QVector3D deltaX(20.0, 20.0, 0.0);
    int nheliostats = 10;

    painter.translate(500.0, 900.0);
    painter.scale(1.0, -1.0);

    for (int index = 0; index < nheliostats; index++)
    {
        heliostat_center = heliostat_center + deltaX * index;
        HeliostatRectangular heliostat(10.0, 10.0, heliostat_center, heliostat_normal);
        heliostat.DrawSunProjection(solar_vector, m_pBuffer);
    }

    painter.drawPixmap(-500,0,*m_pBuffer);
}
