#ifndef SNAKE_H
#define SNAKE_H

#include <QObject>
#include <QGraphicsItem>
#include "constants.h"

class ControlCenter;

class snake: public QGraphicsItem
{
public:
    friend class ControlCenter;
    friend class subWidget;
    snake(ControlCenter &Game, int player);
    bool contains(const QPointF &p) const;
    ~snake();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QPainterPath shape() const;
    void advance(int phase);
    void handleCollisions();
    void redirection(Direction di);
    void speedUp();
    void speedDown();
    void lifePlus();
    void Invincible2s();
    void Inevitalbe2s();

protected:
    ControlCenter &game;
    Direction dir;
    Direction preDir;
    QList<QPointF> SnakeBody;
    QPointF head;
    QPainterPath *headPath;
    int lengToGrow;        
    int deltaTime;
    int speed;
    int Times;    
    int playerNumber;
    int life;
    int score;
    int timeRecorder, SpeedUpRecorder, SpeedDownRecorder;
    bool ifSpeedUp,ifSpeedDown;
    bool invincible;
    bool inevitable;
    QColor color, color1;

    void moveUP();
    void moveDOWN();
    void moveLEFT();
    void moveRIGHT();
    void colorSwitchBack();
};

#endif // SNAKE_H
