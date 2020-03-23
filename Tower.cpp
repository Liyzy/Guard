#include "tower.h"
#include "enemy.h"
#include "bullet.h"
#include "mainwindow.h"
#include "collide.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>
#include <QString>
#include <QPixmap>

const QSize Tower::ms_fixedSize(42, 42);

Tower::Tower(QPoint pos, MainWindow *game, const QPixmap &sprite,const QPixmap &sprite2,const QPixmap &sprite3)
    : TowerLevel(1)
    , m_attackRange(60)
    , m_damage(10)
    , m_fireRate(1000)
    , m_rotationSprite(0.0)
    , m_chooseEnemy(NULL)
    , m_game(game)
    , m_pos(pos)
    , m_sprite(sprite)
    , m_sprite2(sprite2)
    , m_sprite3(sprite3)
{
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootBullet()));
}

void Tower::TowerLevelUp()
{
    TowerLevel++;
    m_damage+=3*TowerLevel;
    m_fireRate-=50;
}

Tower::~Tower()
{
    delete m_fireRateTimer;
    m_fireRateTimer = NULL;
}

void Tower::checkEnemyInRange()
{
    if (m_chooseEnemy)
    {
        // 这种情况下,需要旋转炮台对准敌人
        // 向量标准化
        QVector2D normalized(m_chooseEnemy->pos() - m_pos);
        normalized.normalize();
        m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) - 90;

        // 如果敌人脱离攻击范围
        if (!collideWithCircle(m_pos, m_attackRange, m_chooseEnemy->pos(), 1))
            lostSightOfEnemy();
    }
    else
    {
        // 遍历敌人,看是否有敌人在攻击范围内
        QList<Enemy *> enemyList = m_game->enemyList();
        foreach (Enemy *enemy, enemyList)
        {
            if (collideWithCircle(m_pos, m_attackRange, enemy->pos(), 1))
            {
                chooseEnemyForAttack(enemy);
                break;
            }
        }
    }
}


void Tower::draw(QPainter *painter)const
{
    painter->save();
    painter->setPen(Qt::white);
    // 绘制攻击范围
    painter->drawEllipse(m_pos, m_attackRange, m_attackRange);

    // 绘制偏转坐标,由中心+偏移=左上
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    // 绘制炮塔并选择炮塔
    painter->translate(m_pos);           //设置坐标原点为m_pos
    painter->rotate(m_rotationSprite);   //坐标系旋转角度为m_rotationSprite
    if(TowerLevel==1)
        painter->drawPixmap(offsetPoint, m_sprite);     //画图
    else if(TowerLevel==2)
        painter->drawPixmap(offsetPoint, m_sprite2);
    else
        painter->drawPixmap(offsetPoint, m_sprite3);
    painter->drawText(offsetPoint,QString("Level %1").arg(TowerLevel));
    painter->restore();                  //坐标原点和坐标系初始化
}

void Tower::attackEnemy()
{
    m_fireRateTimer->start(m_fireRate);
}

void Tower::chooseEnemyForAttack(Enemy *enemy)
{
    m_chooseEnemy = enemy;
    attackEnemy();
    m_chooseEnemy->getAttacked(this);
}

void Tower::shootBullet()
{
    Bullet *bullet = new Bullet(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game);
    bullet->move();
    m_game->addBullet(bullet);
}

void Tower::targetKilled()
{
    if (m_chooseEnemy)
        m_chooseEnemy = NULL;

    m_fireRateTimer->stop();
    m_rotationSprite = 0.0;
}

void Tower::lostSightOfEnemy()
{
    m_chooseEnemy->gotLostSight(this);
    if (m_chooseEnemy)
        m_chooseEnemy = NULL;

    m_fireRateTimer->stop();
    m_rotationSprite = 0.0;
}

