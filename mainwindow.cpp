#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "waypoint.h"
#include "enemy.h"
#include "bullet.h"
#include "clickposition.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtGlobal>
#include <QMessageBox>
#include <QTimer>
#include <QXmlStreamReader>
#include <QtDebug>
#include <QWidget>
#include <QList>
#include <QtDebug>
#include <QElapsedTimer>
#include "music.h"

static const int TowerCost = 300;
int MainWindow::m_waves = 0;

class AudioPlayer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_playerHp(5)
    , m_playrGold(20000)
    , m_gameEnded(false)
    , m_gameWin(false)
{
    ui->setupUi(this);

    loadTowerPositions();
    addWayPoints();

    m_audioPlayer = new AudioPlayer(this);
    m_audioPlayer->startBGM();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);

    // 设置300ms后游戏启动
    QTimer::singleShot(300, this, SLOT(gameStart()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadWave()
{
    if (m_waves >= 2)
        return false;
    WayPoint *startWayPoint = m_wayPointsList.back();  // 最后一个节点是敌人的出生点
    int enemyStartInterval[] = {100, 500, 1000, 1700, 3000, 6000};
    for (int i = 0; i < 6; ++i)
    {
        Enemy *enemy = new Enemy(startWayPoint, this);
        m_enemyList.push_back(enemy);
        QTimer::singleShot(enemyStartInterval[i], enemy, SLOT(doActive()));
    }
    return true;
}

void MainWindow::loadTowerPositions()
{
    QPoint pos[]={
        QPoint(65,220),
        QPoint(155,220),
        QPoint(245,220),
        QPoint(335,220),

        QPoint(100,125),
        QPoint(195,125),
        QPoint(280,125),
        QPoint(370,125),

        QPoint(80,35),
        QPoint(170,35),
        QPoint(260,35),
        QPoint(350,35),
    };
    int len=sizeof(pos)/sizeof(pos[0]);
    for(int i=0; i<len; i++)
            m_towerPositionsList.push_back(pos[i]);
 }
//构建游戏界面
void MainWindow::paintEvent(QPaintEvent *)
{
    //胜利和失败的界面
    if(m_gameEnded  || m_gameWin)
    {
        QPixmap Pix1("../GUARD/images/Defeat.png");
        QPixmap Pix2("../GUARD/images/Victory.png");
        QPainter painter(this);
        if(m_gameEnded)
            painter.drawPixmap(rect(), Pix1);
        else
            painter.drawPixmap(rect(), Pix2);
        return;
    }
//打印所有元素
    QPixmap cachePix("../GUARD/images/Bg.png");
    QPainter cachePainter(&cachePix);

    foreach (const TowerPosition &towerPos, m_towerPositionsList)
        towerPos.draw(&cachePainter);

    foreach (const Tower *tower, m_towersList)
        tower->draw(&cachePainter);

    foreach (const WayPoint *wayPoint, m_wayPointsList)
        wayPoint->draw(&cachePainter);

    foreach (const Enemy *enemy, m_enemyList)
        enemy->draw(&cachePainter);

    foreach (const Bullet *bullet, m_bulletList)
        bullet->draw(&cachePainter);

    foreach (const ClickPosition *clickposition, m_clickpositionNow)
        if(clickposition->hasClick())
            clickposition->draw(&cachePainter);

    drawWave(&cachePainter);
    drawHP(&cachePainter);
    drawPlayerGold(&cachePainter);
    drawPause(&cachePainter);

    QPainter painter(this);
    painter.drawPixmap(0, 0,cachePix.width(),cachePix.height(), cachePix);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint pressPos = event->pos();
    if(pressPos.x()>210 && pressPos.x()<310 && pressPos.y()>300 && pressPos.y()<325)
    {
        QElapsedTimer t;
        t.start();
        while(t.elapsed()<1000)
            QCoreApplication::processEvents();
    }
    auto it = m_towerPositionsList.begin();
    while (it != m_towerPositionsList.end())
    {
        if (canBuyTower() && it->containPoint(pressPos) && !it->hasTower())
        {
            m_audioPlayer->playSound(TowerPlaceSound);
            m_playrGold -= TowerCost;
            it->setHasTower(true);
            Tower *tower = new Tower(it->centerPos(), this);
            m_towersList.push_back(tower);
            update();
            break;
        }
        else if(it->containPoint(pressPos)&&it->hasTower())
        {
            ClickPosition *clickposition=new ClickPosition(it->centerPos()+QPoint(-35,-52));
            foreach (ClickPosition *clickposition, m_clickpositionNow)
                clickposition->setHasClick(false);
            clickposition->setHasClick(true);
            m_clickpositionNow.push_back(clickposition);
            update();
            break;
        }
        else if((pressPos.x()>it->centerPos().x()-35)&&(pressPos.x()<it->centerPos().x()+15)&&(pressPos.y()>it->centerPos().y()-52)&&(pressPos.y()<it->centerPos().y()-22))
        {
            //链表中有元素&&链表最后一个元素显示了&&点在LevelUp区域&&有钱     顺序很重要
            if(m_clickpositionNow.size()>0 && m_clickpositionNow[m_clickpositionNow.size()-1]->hasClick() && m_clickpositionNow[m_clickpositionNow.size()-1]->containLevelUp(pressPos))
            {
                int i=0;
                for(i=0;i<m_towersList.size();++i)
                {
                    if((pressPos.x()>m_towersList[i]->m_pos.x()-35)&&(pressPos.x()<m_towersList[i]->m_pos.x()-10)&&(pressPos.y()>m_towersList[i]->m_pos.y()-52)&&(pressPos.y()<m_towersList[i]->m_pos.y()-22))
                        break;
                }
                if(m_playrGold>=100+50*(m_towersList[i]->TowerLevel+1))
                {
                    m_towersList[i]->TowerLevelUp();
                    m_playrGold-=100+50*m_towersList[i]->TowerLevel;
                }

            }
            //链表中有元素&&链表最后一个元素显示了&&点在Sale区域        顺序！！
            else if(m_clickpositionNow.size()>0 && m_clickpositionNow[m_clickpositionNow.size()-1]->hasClick())
            {
                int j=0;
                for(j=0;j<m_towersList.size();++j)
                {
                    if((pressPos.x()>m_towersList[j]->m_pos.x()-10)&&(pressPos.x()<m_towersList[j]->m_pos.x()+15)&&(pressPos.y()>m_towersList[j]->m_pos.y()-52)&&(pressPos.y()<m_towersList[j]->m_pos.y()-22))
                        break;
                }
                m_playrGold+=150+50*m_towersList[j]->TowerLevel;
                m_towersList.erase(m_towersList.begin()+j);
                it->setHasTower(false);

            }
            foreach (ClickPosition *clickposition, m_clickpositionNow)
                clickposition->setHasClick(false);
            update();
        }
        ++it;
    }
    if(it==m_towerPositionsList.end())
    {
        foreach (ClickPosition *clickposition, m_clickpositionNow)
            clickposition->setHasClick(false);
        update();
    }

}

bool MainWindow::canBuyTower() const
{
    if (m_playrGold >= TowerCost)
        return true;
    return false;
}

void MainWindow::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(400, 5, 100, 25), QString("WAVE : %1").arg(m_waves + 1));
}

void MainWindow::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(30, 5, 100, 25), QString("HP : %1").arg(m_playerHp));
}

void MainWindow::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(200, 5, 200, 25), QString("GOLD : %1").arg(m_playrGold));
}

void MainWindow::drawPause(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(210,300,100,25),QString("Pause"));
}

void MainWindow::doGameOver()
{
    if (!m_gameEnded)
    {
        // 此处应该切换场景到结束场景
        m_gameEnded = true;

    }
}

void MainWindow::awardGold(int gold)
{
    m_playrGold += gold;
    update();
}

AudioPlayer *MainWindow::audioPlayer() const
{
    return m_audioPlayer;
}

void MainWindow::addWayPoints()
{
    WayPoint *wayPoint1 = new WayPoint(QPoint(420, 285));
    m_wayPointsList.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(35, 285));
    m_wayPointsList.push_back(wayPoint2);
    wayPoint2->setNextWayPoint(wayPoint1);

    WayPoint *wayPoint3 = new WayPoint(QPoint(35, 195));
    m_wayPointsList.push_back(wayPoint3);
    wayPoint3->setNextWayPoint(wayPoint2);

    WayPoint *wayPoint4 = new WayPoint(QPoint(445, 195));
    m_wayPointsList.push_back(wayPoint4);
    wayPoint4->setNextWayPoint(wayPoint3);

    WayPoint *wayPoint5 = new WayPoint(QPoint(445, 100));
    m_wayPointsList.push_back(wayPoint5);
    wayPoint5->setNextWayPoint(wayPoint4);

    WayPoint *wayPoint6 = new WayPoint(QPoint(35, 100));
    m_wayPointsList.push_back(wayPoint6);
    wayPoint6->setNextWayPoint(wayPoint5);
}

void MainWindow::getHpDamage(int damage)
{
    m_audioPlayer->playSound(LifeLoseSound);
    m_playerHp -= damage;
    if (m_playerHp <= 0)
        doGameOver();
}

void MainWindow::removedEnemy(Enemy *enemy)
{
    Q_ASSERT(enemy);

    m_enemyList.removeOne(enemy);
    delete enemy;

    if (m_enemyList.empty())
    {
        ++m_waves;
        if (!loadWave())
        {
            // 游戏胜利转到游戏胜利场景
            m_gameWin = true;

        }
    }
}

void MainWindow::removedBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.removeOne(bullet);
    delete bullet;
}

void MainWindow::addBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.push_back(bullet);
}

void MainWindow::updateMap()
{
    foreach (Enemy *enemy, m_enemyList)
        enemy->move();
    foreach (Tower *tower, m_towersList)
        tower->checkEnemyInRange();
    update();
}


QList<Enemy *> MainWindow::enemyList() const
{
    return m_enemyList;
}

void MainWindow::gameStart()
{
    loadWave();
}

