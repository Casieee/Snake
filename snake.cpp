#include "snake.h"
#include <QPainter>
#include "controlcenter.h"
#include "QDebug"
#include "food.h"
#include <QLabel>

snake::snake(ControlCenter &Game, int player):    
    game(Game),
    head(0,0),
    lengToGrow(3),
    deltaTime(5),
    Times(0),
    playerNumber(player),
    life(1),
    timeRecorder(0),
    SpeedUpRecorder(0),
    SpeedDownRecorder(0),
    ifSpeedUp(false),
    ifSpeedDown(false),
    invincible(false),
    inevitable(false)
{
    headPath = new QPainterPath;
    if(player == 1){
        life = 100;
        dir=DOWN;
        preDir=DOWN;
        color = blue;
        color1 = blue2;
    }
    if(player == 2){
        head = mapFromScene(QPointF(900-block,720-block));
        dir=UP;
        preDir=UP;
        color = pink;
        color1 = deeppink;
    }
}

snake::~snake(){}

bool snake::contains(const QPointF &p) const{
    if(SnakeBody.contains(p)||head == p)
        return true;
    else
        return false;
}

QRectF snake::boundingRect() const{
    qreal minX = head.x();
    qreal minY = head.y();
    qreal maxX = head.x();
    qreal maxY = head.y();
    for (QPointF T: SnakeBody) {
            maxX = T.x() > maxX ? T.x() : maxX;
            maxY = T.y() > maxY ? T.y() : maxY;
            minX = T.x() < minX ? T.x() : minX;
            minY = T.y() < minY ? T.y() : minY;
        }
    QPointF min = mapFromScene(QPointF(minX,minY));
    QPointF max = mapFromScene(QPointF(maxX,maxY));
    QRect bR(min.x(), min.y(), max.x()-min.x()+block, max.y()-min.y()+block);
    return bR;
}

QPainterPath snake::shape() const{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    QRectF Rec(0,0,block,block);
    path.addRect(Rec);
    QRectF r(7.5,7.5,15,15);
    headPath->addRect(r);
    for(QPointF T:SnakeBody){
        QPointF R = mapFromScene(T);
        QRectF Rec(R.x(),R.y(),block,block);
        path.addRect(Rec);
    }
    return path;
}

void snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    painter->save();
    painter->fillPath(shape(),color);
    painter->fillPath(*headPath,color1);
    headPath->clear();
    painter->restore();
}

void snake::handleCollisions(){
    if(game.handleCollisions(head, playerNumber)){
        lengToGrow++;
    }

    if(playerNumber >= 2)
        game.SnakesColliding();

    game.SnakeIntoWall(playerNumber);

    if(SnakeBody.contains(head)){
        game.HeadIntoBody(playerNumber);
    }
}

void snake::Invincible2s(){
    invincible = true;
    timeRecorder = 0;
    lengToGrow = 1 - SnakeBody.length();
    color = gold;
    color1 = Qt::yellow;
}

void snake::Inevitalbe2s(){
    inevitable = true;
    timeRecorder = 0;
    color = darkred;
    color1 = Qt::red;
}

void snake::advance(int phase){
    if(!phase)
        return;

    Times++;
    if(Times%deltaTime != 0) return;

    if(lengToGrow>0) {
        QPointF newPart = head;
        SnakeBody << newPart;
        lengToGrow--;
    }
    else if(lengToGrow < 0) {
        while(lengToGrow < 0){
            SnakeBody.removeFirst();
            lengToGrow++;
        }
        SnakeBody << head;
    }else {
        SnakeBody.removeFirst();
        SnakeBody << head;
    }
    //can't turn back
    if(dir-preDir == 2||preDir-dir ==2)
        dir = preDir;

    switch (dir) {
    case UP: moveUP(); break;
    case DOWN: moveDOWN(); break;
    case LEFT: moveLEFT(); break;
    case RIGHT: moveRIGHT(); break;
    }
    preDir = dir;

    setPos(head);

    if(ifSpeedUp){
        SpeedUpRecorder++;
        if(SpeedUpRecorder == 25){
            deltaTime+=1;
            ifSpeedUp = false;
            SpeedUpRecorder = 0;
        }
    }

    if(ifSpeedDown){
        SpeedDownRecorder++;
        if(SpeedDownRecorder == 25){
            deltaTime-=2;
            ifSpeedDown = false;
            SpeedDownRecorder = 0;
        }
    }

    if(invincible) {
        timeRecorder++;
        if(timeRecorder==10){
            invincible = false;
            if(playerNumber == 1){
                color = blue;
                color1 = blue2;
            }
            if(playerNumber == 2){
                color = pink;
                color1 = deeppink;
            }
        }
    }
    else {
        if(inevitable){
            timeRecorder++;
            if(timeRecorder==15){
                inevitable = false;
                if(playerNumber == 1){
                    color = blue;
                    color1 = blue2;
                }
                if(playerNumber == 2){
                    color = pink;
                    color1 = deeppink;
                }
            }
        }
        handleCollisions();
    }
}

void snake::redirection(Direction di){
    dir = di;
}

void snake::moveUP(){
    head.ry()-= block;
    if(head.y() == -block)
        head.ry() = 720 - block;
}

void snake::moveDOWN(){
    head.ry() += block;
    if(head.y() == 720)
        head.ry() = 0;
}

void snake::moveLEFT(){
    head.rx() -= block;
    if(head.x() == -block){
        head.rx() = 900 - block;
    }
}

void snake::moveRIGHT(){
    head.rx() += block;
    if(head.rx() == 900){
        head.rx() = 0;
    }
}

void snake::speedUp(){
    if(deltaTime!=4){
        ifSpeedUp = true;
        deltaTime-=1;
    }
}

void snake::speedDown(){
    if(deltaTime!=7){
        ifSpeedDown = true;
        deltaTime+=2;
    }
}

void snake::lifePlus(){
    life++;
}
