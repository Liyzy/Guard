#ifndef BULLET_H
#define BULLET_H

#include <QPoint>
#include <QPainter>
#include "enemy.h"
#include "mainwindow.h"
#include <QSize>

class Enemy;
class MainWindow;

class Bullet : QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)

public:
    Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target,
           MainWindow *game, const QPixmap &sprite = QPixmap("../GUARD/images/bullet.png"));
    void draw(QPainter *painter) const;
    void move();
    void setCurrentPos(QPoint pos);
    QPoint currentPos() const;

private slots:
    void hitTarget();

private:
    const QPoint m_startPos; //子弹起始位置，即为炮的位置
    const QPoint m_targetPos;  //目标位置，敌人的位置
    const QPixmap m_sprite;
    QPoint m_currentPos;
    Enemy * m_target;
    MainWindow * m_game;
    int m_damage;

    static const QSize ms_fixedSize;
};

#endif // BULLET_H
