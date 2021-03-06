#include "snake.h"
#include <QPainter>
#include "controlcenter.h"
#include "QDebug"
#include "food.h"
#include <QLabel>
#include <QColor>

snake::snake(ControlCenter &Game, int player):    
    game(Game),
    head(0,0),
    lengToGrow(3),
    deltaTime(5),
    speed(2),
    Times(0),
    playerNumber(player),
    life(1),
    score(0),
    jetFuel(0),
    go(false),
    timeRecorder(0),
    SpeedUpRecorder(0),
    SpeedDownRecorder(0),
    ifSpeedUp(false),
    ifSpeedDown(false),
    invincible(false),
    inevitable(false),
    render(false)
{
    headPath = new QPainterPath;
    if(player == 1){
        dir=DOWN;
        preDir=DOWN;
        defaultColor = blue;
        defaultColor1 = blue2;
        color = defaultColor;
        color1 = defaultColor1;
    }
    if(player == 2){
        head = mapFromScene(QPointF(900-block,720-block));
        dir=UP;
        preDir=UP;
        defaultColor = pink;
        defaultColor1 = deeppink;
        color = defaultColor;
        color1 = defaultColor1;
    }
    setAcceptDrops(false);
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

void snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    painter->save();
    painter->setPen(color);
    if (!render) {
        painter->setBrush(color);
        QRectF Rec(0,0,block,block);
        painter->drawRect(Rec);
        for(QPointF T:SnakeBody){
            QPointF R = mapFromScene(T);
            QRectF Rec(R.x(),R.y(),block,block);
            painter->drawRect(Rec);
        }

        QRectF r(block/4,block/4,block/2,block/2);
        painter->setBrush(color1);
        painter->drawRect(r);
    }
    else {
        int leng = SnakeBody.length();
        int r1 = color.red(), r2 = color1.red(), deltaR = (r1 - r2)/leng;
        int g1 = color.green(), g2 = color1.green(), deltaG = (g1 - g2)/leng;
        int b1 = color.blue(), b2 = color1.blue(), deltaB = (b1 - b2)/leng;

        for (int i = 0; i < leng; i++) {
            QPointF R = mapFromScene(SnakeBody[i]);
            painter->setBrush(QColor(r1 - i*deltaR, g1 - i*deltaG, b1 - i*deltaB));
            painter->drawRect(QRect(R.x(),R.y(),block,block));
        }
        painter->setBrush(color1);
        painter->drawRect(0,0,block,block);
    }
    painter->restore();
}

void snake::handleCollisions(){
    if(game.EatFood(head, playerNumber)){
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

    if(go){
        Sprint();
        go = false;
    }
    if(!moves.empty()){
        dir = moves.takeFirst();
    }
    if(lengToGrow>0) {
        QPointF newPart = head;
        SnakeBody << newPart;
        score++;
        lengToGrow--;
    }
    else if(lengToGrow < 0) {
        while(lengToGrow < 0){
            SnakeBody.removeFirst();
            lengToGrow++;
        }
        SnakeBody << head;
        score = 0;
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
        if(SpeedUpRecorder == 35){
            deltaTime+=1;
            speed--;
            ifSpeedUp = false;
            SpeedUpRecorder = 0;
        }
    }

    if(ifSpeedDown){
        SpeedDownRecorder++;
        if(SpeedDownRecorder == 35){
            deltaTime-=1;
            speed++;
            ifSpeedDown = false;
            SpeedDownRecorder = 0;
        }
    }

    if(invincible) {
        timeRecorder++;
        switch (timeRecorder) {
        case 6: case 10: colorSwitchBack(); break;
        case 8: case 12: color = gold; color1 = Qt::yellow; break;
        case 14: colorSwitchBack(); break;
        case 15: invincible = false;
        }
    }
    else {
        if(inevitable){
            timeRecorder++;
            switch (timeRecorder) {
            case 10: case 14: colorSwitchBack(); break;
            case 12: case 16: color = darkred; color1 = Qt::red; break;
            case 18: colorSwitchBack(); break;
            case 19: inevitable = false;
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
        speed++;
    }
}

void snake::speedDown(){
    if(deltaTime!=6){
        ifSpeedDown = true;
        deltaTime+=1;
        speed--;
    }
}

void snake::lifePlus(){
    life++;
}

void snake::colorSwitchBack(){
    color = defaultColor;
    color1 = defaultColor1;
}

void snake::Sprint(){
    if(!jetFuel)
        return;

    for(int i = 0; i < 3; ++i){
        SnakeBody.removeFirst();
        SnakeBody << head;
        switch (dir) {
        case UP: moveUP(); break;
        case DOWN: moveDOWN(); break;
        case LEFT: moveLEFT(); break;
        case RIGHT: moveRIGHT(); break;
        }
    }
    jetFuel--;
}
