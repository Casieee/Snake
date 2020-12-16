#include "aisnake.h"
#include "controlcenter.h"
#include <QDebug>

AIsnake::AIsnake(ControlCenter &Game, int player):
    snake(Game, player)
{
    head = mapFromScene(QPointF(900-block,720-block));
    color = lightGray;
    color1 = deepGray;
    dir = UP;
    preDir = UP;
}

bool AIsnake::pathIntoBody(QPointF p){
    if(this->SnakeBody.contains(p))
        return true;
    else return false;
}

void AIsnake::advance(int phase){
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

    this->game.automove(head, dir);

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
            deltaTime-=2;
            ifSpeedUp = false;
            SpeedUpRecorder = 0;
        }
    }

    if(ifSpeedDown){
        SpeedDownRecorder++;
        if(SpeedDownRecorder == 25){
            deltaTime+=1;
            ifSpeedDown = false;
            SpeedDownRecorder = 0;
        }
    }

    if(invincible) {
        timeRecorder++;
        if(timeRecorder==10){
            invincible = false;
            color = lightGray;
            color1 = deepGray;
        }
    }
    else {
        if(inevitable){
            timeRecorder++;
            if(timeRecorder==15){
                inevitable = false;
                color = lightGray;
                color1 = deepGray;
            }
        }
        handleCollisions();
    }
}
