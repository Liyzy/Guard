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
    // ���˵���һ��·�߹յ�
    if(collideWithCircle(m_pos, 1, m_destinationWayPoint->pos(), 1))
    {
        // ���Ҵ�����һ���յ�
        if(m_destinationWayPoint->nextWayPoint())
        {
            m_pos = m_destinationWayPoint->pos();
            m_destinationWayPoint = m_destinationWayPoint->nextWayPoint();
        }
        // �������
        else
        {
            m_game->getHpDamage();
            m_game->removedEnemy(this);
            return ;
        }
    }
    // ��·��
    QPoint targetPoint = m_destinationWayPoint->pos();
    double movementSpeed = m_walkingSpeed;
    // ������׼��  (1,0)(0,1)(0,-1)(-1,0)
    QVector2D normalized(targetPoint-m_pos);
    normalized.normalize();
    // ˢ�µ���λ��
    m_pos = m_pos + normalized.toPoint() * movementSpeed;
    // ������ת�Ƕ�
    m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) + 180;
}

void Enemy::getRemoved()
{
    if (m_attackedTowersList.empty())
        return;
    foreach (Tower *attacker, m_attackedTowersList)
        attacker->targetKilled();
    // ֪ͨgame,�˵����Ѿ�����
    m_game->removedEnemy(this);
}

void Enemy::getDamage(int damage)
{
    m_game->audioPlayer()->playSound(LaserShootSound);
    m_currentHp -= damage;
    // ����,��Ҫ�Ƴ�
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

// ���������Ѿ������˹�����Χ
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
    // ����Ѫ������ɫ�����ʾ��Ѫ������ɫ�����ʾ��ǰѪ��
    painter->setPen(Qt::NoPen); //������ʽ
    painter->setBrush(Qt::red);
    QRect healthBarAllRect(healthBarPoint, QSize(Health_Bar_Width, 2));// �������δ���Ѫ��
    painter->drawRect(healthBarAllRect);
    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);
    // ����ƫת����,ƫ��=����-���ģ�������Ϊ(0,0)
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    // ���Ƶ���
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}


