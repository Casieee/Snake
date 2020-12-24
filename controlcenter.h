#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include <QObject>
#include <QGraphicsScene>
#include <QObject>
#include <QTimer>
#include <QRectF>
#include "constants.h"
#include <QMouseEvent>

class AIsnake;
class snake;
class food;
class Bricks;
class subwidget;

class ControlCenter: public QObject
{
    Q_OBJECT
public:
    friend class snake;
    friend class subWidget;
    friend class AIsnake;
    ControlCenter(QGraphicsScene &scene, QObject *parent = 0, int num = 1, bool AI = false);
    ~ControlCenter();    

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
    void blink();

private:
    void SnakeIntoWall(int player);
    void HeadIntoBody(int player);
    bool eventFilter(QObject *object, QEvent *event);
    void KeyPressed(QKeyEvent* event);
    bool EatFood(QPointF head, int player);

    void automove(QPointF head, Direction &dir);
    bool IntoItem(QPointF p);

    void judgeWinner(int player);
    void GG();
    int winner;

    void newWall();
    void newFood();
    QPointF randomPoint();
    void addNewFood(food* f);
    bool XDirection(QPointF head, QPointF target, Direction &dir);
    bool YDirection(QPointF head, QPointF target, Direction &dir);
    void checkCheats(QVector<Direction> &directs, int player);
    void mousePress(QGraphicsSceneMouseEvent* e);

    QTimer timer;
    QList<food*> Food;
    food* originFood[3];
    int foodNum;
    QGraphicsScene &Scene;
    snake *s[3];
    AIsnake *ai;
    bool ifAI;
    int playerNum;
    QList<Bricks*> Wall;
    Bricks* originWall[5];
    QVector<Direction> cheats;
    int wallNum;
    bool editable;
    QGraphicsItem *movingItem;
    bool ifMoving;
    int blinkCounter;
};

#endif // CONTROLCENTER_H
