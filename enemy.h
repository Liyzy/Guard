#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QMainWindow>
#include <QSize>
#include "WayPoint.h"
#include "Tower.h"
#include "mainwindow.h"

class Tower;
class MainWindow;

class Enemy :public QObject
{
    Q_OBJECT
public:
    Enemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite = QPixmap("../GUARD/images/enemy.png"));
    ~Enemy();

    void move();
    void getDamage(int damage);
    void getRemoved();
    void getAttacked(Tower *attacker);
    void gotLostSight(Tower *attacker);
    QPoint pos() const;

    void draw(QPainter *painter) const;

public slots:
    void doActive();

private:
    int m_maxHp;    //���Ѫ��
    int m_currentHp;  //��ǰѪ��
    double m_walkingSpeed; // �ƶ��ٶ�
    bool m_active; // �Ƿ�ɶ�
    QPoint m_pos;  // λ����Ϣ
    const QPixmap m_sprite;
    WayPoint *m_destinationWayPoint;  // ·����Ϣ
    double m_rotationSprite;  // ͼƬ��ת
    MainWindow * m_game;
    QList<Tower *> m_attackedTowersList;  // �������˵�������

    static const QSize ms_fixedSize;
};

#endif // ENEMY_H
