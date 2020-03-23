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
    int	m_attackRange;	// 代表塔可以攻击到敌人的距离
    int	m_damage;		// 代表攻击敌人时造成的伤害
    int	m_fireRate;		// 代表再次攻击敌人的时间间隔
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
