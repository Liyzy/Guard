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
    const QPoint m_startPos; //�ӵ���ʼλ�ã���Ϊ�ڵ�λ��
    const QPoint m_targetPos;  //Ŀ��λ�ã����˵�λ��
    const QPixmap m_sprite;
    QPoint m_currentPos;
    Enemy * m_target;
    MainWindow * m_game;
    int m_damage;

    static const QSize ms_fixedSize;
};

#endif // BULLET_H
