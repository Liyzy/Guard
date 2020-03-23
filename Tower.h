#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class Enemy;
class MainWindow;
class QTimer;

class Tower : QObject
{
    Q_OBJECT
public:
    Tower(QPoint pos, MainWindow *game, const QPixmap &sprite = QPixmap("../GUARD/images/tower1.png"),const QPixmap &sprite2=QPixmap("../GUARD/images/tower2.png"),const QPixmap &sprite3=QPixmap("../GUARD/images/tower3.png"));
    ~Tower();

    void draw(QPainter *painter)const;
    void checkEnemyInRange();
    void targetKilled();
    void attackEnemy();
    void chooseEnemyForAttack(Enemy *enemy);


    void lostSightOfEnemy();
    void TowerLevelUp();
    int TowerLevel;
    const QPoint	m_pos;

private slots:
    void shootBullet();

private:
    int	m_attackRange;	// ���������Թ��������˵ľ���
    int	m_damage;		// ����������ʱ��ɵ��˺�
    int	m_fireRate;		// �����ٴι������˵�ʱ����
    qreal m_rotationSprite;

    Enemy *	m_chooseEnemy;
    MainWindow *m_game;
    QTimer *m_fireRateTimer;

    const QPixmap	m_sprite;
    const QPixmap   m_sprite2;
    const QPixmap   m_sprite3;

    static const QSize ms_fixedSize;
};

#endif // TOWER_H
