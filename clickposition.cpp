#include"clickposition.h"
#include<QPainter>

//Ä£·ÂËþposition
const QSize ClickPosition::ms_fixedSize(75,30);

ClickPosition::ClickPosition(QPoint pos, const QPixmap &sprite)
    :m_hasClick(false)
    ,m_sprite(sprite)
    ,m_pos(pos)
{

}


bool ClickPosition::hasClick() const
{
    return m_hasClick;
}


void ClickPosition::setHasClick(bool hasClick/* = true*/)
{
    m_hasClick = hasClick;
}

void ClickPosition::draw(QPainter *painter) const
{
    painter->drawPixmap(m_pos.x(), m_pos.y(), m_sprite);
}

bool ClickPosition::containLevelUp(const QPoint &pos) const
{
    bool isXInHere = m_pos.x() < pos.x() && pos.x() < (m_pos.x() + ms_fixedSize.width()/3);
    bool isYInHere = m_pos.y() < pos.y() && pos.y() < (m_pos.y() + ms_fixedSize.height());
    return isXInHere && isYInHere;
}

bool ClickPosition::containSale(const QPoint &pos) const
{
    bool isXInHere = m_pos.x() + ms_fixedSize.width()/3 < pos.x() && pos.x() < (m_pos.x() + 2*ms_fixedSize.width()/3);
    bool isYInHere = m_pos.y() < pos.y() && pos.y() < (m_pos.y() + ms_fixedSize.height());
    return isXInHere && isYInHere;
}

