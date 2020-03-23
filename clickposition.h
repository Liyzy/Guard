#ifndef CLICKPOSITION_H
#define CLICKPOSITION_H

#include "towerposition.h"
#include <QPoint>
#include <QSize>
#include <QPixmap>
class QPainter;

class ClickPosition :public QObject
{
public:
    ClickPosition(QPoint pos, const QPixmap &sprite = QPixmap("../GUARD/images/UpLevel.png"));

    void setHasClick(bool hasTower = true);
    bool hasClick() const;
    bool containLevelUp(const QPoint &pos) const;
    bool containSale(const QPoint &pos) const;

    void draw(QPainter *painter) const;

private:
    bool m_hasClick;
    QPoint m_pos;
    QPixmap	m_sprite;

    static const QSize ms_fixedSize;
};

#endif // CLICKPOSITION_H
