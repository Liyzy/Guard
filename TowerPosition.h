#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include <QPoint>
#include <Qsize>
#include <QPainter>

class Tower;
class QPiont;

class TowerPosition
{
public:
    TowerPosition(QPoint pos, const QPixmap & sprite=QPixmap("../GUARD/images/open_spot.png"));

    void setHasTower(bool hasTower = true);//这里使用了默认参数
    bool hasTower() const;
    const QPoint centerPos() const;//炮台的中心点坐标
    bool containPoint(const QPoint & pos) const;
    void draw(QPainter * painter) const;
    QPoint getPos();

private:
    QPoint m_pos;//炮台的左上角坐标
    bool m_hasTower;//是否有炮
    QPixmap m_sprite;//保存绘制的图片
    static const QSize ms_fixedSize;//矩形炮台的大小
};
#endif // TOWERPOSITION_H
