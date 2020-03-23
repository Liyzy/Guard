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
    int m_maxHp;    //最大血量
    int m_currentHp;  //当前血量
    double m_walkingSpeed; // 移动速度
    bool m_active; // 是否可动
    QPoint m_pos;  // 位置信息
    const QPixmap m_sprite;
    WayPoint *m_destinationWayPoint;  // 路线信息
    double m_rotationSprite;  // 图片旋转
    MainWindow * m_game;
    QList<Tower *> m_attackedTowersList;  // 攻击敌人的炮链表

    static const QSize ms_fixedSize;
};

#endif // ENEMY_H
