#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include <QObject>
#include <QGraphicsScene>
#include <QObject>
#include <QTimer>
#include <QRectF>
#include "constants.h"

class AIsnake;
class snake;
class food;
class subwidget;

class ControlCenter: public QObject
{
    Q_OBJECT
public:
    friend class snake;
    friend class subWidget;
    ControlCenter(QGraphicsScene &scene, QObject *parent = 0, int num = 1, bool AI = false);
    ~ControlCenter();
    void SnakeIntoWall(int player);
    void HeadIntoBody(int player);
    bool eventFilter(QObject *object, QEvent *event);
    void KeyPressed(QKeyEvent* event);
    bool handleCollisions(QPointF head, int player);
    void automove(QPointF head, Direction &dir);
    bool IntoItem(QPointF p);
    void judgeWinner(int player);
    void GG();

    int winner;

signals:
    void turnToPausePage();
    void gameOver();
    void win();
    void updateStatus();

public slots:
    void start();
    void pause();
    void SnakesColliding();
    void sendUpdate();

private:
    QPointF randomPoint();
    void addNewFood(food* f);
    bool XDirection(QPointF head, QPointF target, Direction &dir);
    bool YDirection(QPointF head, QPointF target, Direction &dir);

    QTimer timer;
    food *f[8];
    int foodNum;
    QGraphicsScene &Scene;
    snake *s[3];
    AIsnake *ai;
    bool ifAI;
    int playerNum;
    QGraphicsRectItem *wall[10];
    int wallNum;
};

#endif // CONTROLCENTER_H
