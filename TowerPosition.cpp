#include "TowerPosition.h"
#include <QPainter>
#include <QPixmap>

class QPoint;

const QSize TowerPosition::ms_fixedSize(44, 44);

TowerPosition::TowerPosition(QPoint pos, const QPixmap &sprite)
        : m_hasTower(false), m_pos(pos), m_sprite(sprite)
{
}

const QPoint TowerPosition::centerPos() const  //获得炮台中心坐标
{
        QPoint offsetPoint(ms_fixedSize.width()/ 2,ms_fixedSize.height() / 2);
        return m_pos + offsetPoint;
}

bool TowerPosition::containPoint(const QPoint &pos) const  //判断点击的(x,y)是否在可放置范围内
{
        bool isXInHere = m_pos.x() < pos.x() && pos.x() < (m_pos.x() + ms_fixedSize.width());
        bool isYInHere = m_pos.y() < pos.y() && pos.y() < (m_pos.y() + ms_fixedSize.height());
        return isXInHere && isYInHere;
}

bool TowerPosition::hasTower() const
{
        return m_hasTower;
}

void TowerPosition::setHasTower(bool hasTower)
{
        m_hasTower = hasTower;
}

void TowerPosition::draw(QPainter *painter) const
{
    painter->drawPixmap(m_pos.x(), m_pos.y(),m_sprite.width(), m_sprite.height(),m_sprite);
}

QPoint TowerPosition::getPos()
{
    return m_pos;
}
