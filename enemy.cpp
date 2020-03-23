#include "enemy.h"
#include "WayPoint.h"
#include "mainwindow.h"
#include "collide.h"
#include <QPoint>
#include <QtMath>
#include <QVector2D>
#include <QPixmap>
#include "music.h"

const QSize Enemy::ms_fixedSize(52, 52);

Enemy::Enemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite)
    :QObject(0),m_pos(startWayPoint->pos()),m_sprite(sprite), m_destinationWayPoint(startWayPoint->nextWayPoint())
{
    m_maxHp = 50 + MainWindow::m_waves * 20;
    m_currentHp = 50 + MainWindow::m_waves * 20;
    m_active = false;
    m_walkingSpeed = 1.0;
    m_destinationWayPoint = startWayPoint->nextWayPoint();
    m_rotationSprite = 0.0;
    m_game = game;
}

Enemy::~Enemy()
{
    m_attackedTowersList.clear();
    m_destinationWayPoint = NULL;
    m_game = NULL;
}

void Enemy::doActive()
{
    m_active=true;
}

void Enemy::move()
{
    if(! m_active)
        return ;
    // 敌人到了一个路线拐点
    if(collideWithCircle(m_pos, 1, m_destinationWayPoint->pos(), 1))
    {
        // 并且存在下一个拐点
        if(m_destinationWayPoint->nextWayPoint())
        {
            m_pos = m_destinationWayPoint->pos();
            m_destinationWayPoint = m_destinationWayPoint->nextWayPoint();
        }
        // 进入基地
        else
        {
            m_game->getHpDamage();
            m_game->removedEnemy(this);
            return ;
        }
    }
    // 在路上
    QPoint targetPoint = m_destinationWayPoint->pos();
    double movementSpeed = m_walkingSpeed;
    // 向量标准化  (1,0)(0,1)(0,-1)(-1,0)
    QVector2D normalized(targetPoint-m_pos);
    normalized.normalize();
    // 刷新敌人位置
    m_pos = m_pos + normalized.toPoint() * movementSpeed;
    // 计算旋转角度
    m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) + 180;
}

void Enemy::getRemoved()
{
    if (m_attackedTowersList.empty())
        return;
    foreach (Tower *attacker, m_attackedTowersList)
        attacker->targetKilled();
    // 通知game,此敌人已经阵亡
    m_game->removedEnemy(this);
}

void Enemy::getDamage(int damage)
{
    m_game->audioPlayer()->playSound(LaserShootSound);
    m_currentHp -= damage;
    // 阵亡,需要移除
    if (m_currentHp <= 0)
    {
        m_game->audioPlayer()->playSound(EnemyDestorySound);
        m_game->awardGold(200);
        getRemoved();
    }
}

void Enemy::getAttacked(Tower *attacker)
{
    m_attackedTowersList.push_back(attacker);
}

// 表明敌人已经逃离了攻击范围
void Enemy::gotLostSight(Tower *attacker)
{
    m_attackedTowersList.removeOne(attacker);
}

QPoint Enemy::pos() const
{
    return m_pos;
}

void Enemy::draw(QPainter *painter) const
{
    if(! m_active)
        return;
    static const int Health_Bar_Width = 20;
    painter->save();
    QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, -ms_fixedSize.height() / 3);
    // 绘制血条，红色方框表示总血量，绿色方框表示当前血量
    painter->setPen(Qt::NoPen); //画布格式
    painter->setBrush(Qt::red);
    QRect healthBarAllRect(healthBarPoint, QSize(Health_Bar_Width, 2));// 创建矩形代表血条
    painter->drawRect(healthBarAllRect);
    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);
    // 绘制偏转坐标,偏移=左上-中心，中心设为(0,0)
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    // 绘制敌人
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}


