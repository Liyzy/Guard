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

    static int	m_waves; //����

private slots:
    void updateMap();
    void gameStart();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent * event);

private:
    void loadTowerPositions();
    bool canBuyTower() const;
    void addWayPoints(); //����·�߹յ�
    bool loadWave(); //���ص��˲���
    void drawWave(QPainter *painter);
    void drawHP(QPainter *painter);
    void drawPause(QPainter *painter);
    void drawPlayerGold(QPainter *painter);
    void doGameOver();

private:
    Ui::MainWindow *ui;
    QList<TowerPosition> m_towerPositionsList; //������̨��������Ϣ
    QList<WayPoint*> m_wayPointsList; //����յ��������Ϣ
    QList<Enemy*> m_enemyList;
    QList<Tower*> m_towersList;  //�����ڵ�������Ϣ
    int	m_playerHp; // ����Ѫ��
    int	m_playrGold; //��ҽ��
    QList<ClickPosition *>  m_clickpositionNow;
    QList<Bullet *>	m_bulletList;
    AudioPlayer* m_audioPlayer;
    bool m_gameEnded;
    bool m_gameWin;
};
#endif // MAINWINDOW_H
