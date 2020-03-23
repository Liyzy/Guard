#include "WayPoint.h"
#include <QPoint>
#include <QPainter>
#include "mainwindow.h"

WayPoint::WayPoint(QPoint pos): m_pos(pos),m_nextWayPoint(NULL)
{}

void WayPoint::setNextWayPoint(WayPoint *nextPoint)
{
    m_nextWayPoint=nextPoint;
}

WayPoint* WayPoint::nextWayPoint() const
{
    return m_nextWayPoint;
}

const QPoint WayPoint::pos() const
{
    return m_pos;
}

//绘制拐点
void WayPoint::draw(QPainter *painter) const
{
    painter->save();
    painter->setPen(QColor(0,255,0));
    painter->drawEllipse(m_pos,4,4);
    painter->drawEllipse(m_pos,2,2);
    //两个拐点之间加线
    if(m_nextWayPoint)
        painter->drawLine(m_pos,m_nextWayPoint->pos());
    painter->restore();
}
