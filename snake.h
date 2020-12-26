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
    void advance(int phase);


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
    int jetFuel;
    bool go;
    int timeRecorder, SpeedUpRecorder, SpeedDownRecorder;
    bool ifSpeedUp,ifSpeedDown;
    bool invincible;
    bool inevitable;
    bool render;
    QColor color, color1;
    QColor defaultColor, defaultColor1;
    QList<Direction> moves;
    QVector<Direction> useCheat;

    void moveUP();
    void moveDOWN();
    void moveLEFT();
    void moveRIGHT();
    void colorSwitchBack();
    void handleCollisions();
    void redirection(Direction di);
    void speedUp();
    void speedDown();
    void lifePlus();
    void Invincible2s();
    void Inevitalbe2s();
    void Sprint();
};

#endif // SNAKE_H
