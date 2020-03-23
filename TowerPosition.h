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

    void setHasTower(bool hasTower = true);//����ʹ����Ĭ�ϲ���
    bool hasTower() const;
    const QPoint centerPos() const;//��̨�����ĵ�����
    bool containPoint(const QPoint & pos) const;
    void draw(QPainter * painter) const;
    QPoint getPos();

private:
    QPoint m_pos;//��̨�����Ͻ�����
    bool m_hasTower;//�Ƿ�����
    QPixmap m_sprite;//������Ƶ�ͼƬ
    static const QSize ms_fixedSize;//������̨�Ĵ�С
};
#endif // TOWERPOSITION_H
