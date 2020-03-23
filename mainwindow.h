#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TowerPosition.h"
#include "Tower.h"
#include <QList>
#include <WayPoint.h>
#include "enemy.h"
#include "bullet.h"
#include "clickposition.h"

class Tower;
class Bullet;
class AudioPlayer;
class Opration;
class Enemy;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void getHpDamage(const int damage=1);
    void removedEnemy(Enemy * enemy);
    QList<Enemy *> enemyList() const;
    void removedBullet(Bullet *bullet);
    void addBullet(Bullet *bullet);
    void awardGold(int gold);
    AudioPlayer* audioPlayer() const;

    static int	m_waves; //波数

private slots:
    void updateMap();
    void gameStart();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent * event);

private:
    void loadTowerPositions();
    bool canBuyTower() const;
    void addWayPoints(); //设置路线拐点
    bool loadWave(); //加载敌人波数
    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawPause(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
    void doGameOver();

private:
    Ui::MainWindow *ui;
    QList<TowerPosition> m_towerPositionsList; //管理炮台的所有信息
    QList<WayPoint*> m_wayPointsList; //管理拐点的所有信息
    QList<Enemy*> m_enemyList;
    QList<Tower*> m_towersList;  //管理炮的所有信息
    int	m_playerHp; // 基地血量
    int	m_playrGold; //玩家金币
    QList<ClickPosition *>  m_clickpositionNow;
    QList<Bullet *>	m_bulletList;
    AudioPlayer* m_audioPlayer;
    bool m_gameEnded;
    bool m_gameWin;
};
#endif // MAINWINDOW_H
