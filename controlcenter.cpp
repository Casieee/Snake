#include "controlcenter.h"
#include "snake.h"
#include "QEvent"
#include "QKeyEvent"
#include "food.h"
#include <QDebug>
#include <QWidget>
#include <QLabel>
#include "aisnake.h"
#include <QPushButton>
#include <QQueue>

ControlCenter::ControlCenter(QGraphicsScene &scene, QObject *parent, int num, bool AI):
    QObject(parent),
    winner(1),
    foodNum(2),
    Scene(scene),
    ifAI(AI),
    playerNum(num),
    wallNum(5)
{
    timer.start(15);
    srand((unsigned)time(NULL));

    wall[0] = new QGraphicsRectItem(180, 150, block*2, block*2);
    wall[1] = new QGraphicsRectItem(660, 150, block*2, block*2);
    wall[2] = new QGraphicsRectItem(180, 510, block*2, block*2);
    wall[3] = new QGraphicsRectItem(660, 510, block*2, block*2);
    wall[4] = new QGraphicsRectItem(420, 330, block*2 ,block*2);
    for(int i = 0 ; i<wallNum; ++i){
        if(wall[i] == nullptr) continue;
        wall[i]->setBrush(Qt::black);
        Scene.addItem(wall[i]);
    }

    f[0] = new food(QPointF(210,360));
    Scene.addItem(f[0]);
    f[1] = new food(QPointF(660,360));
    Scene.addItem(f[1]);

    if(playerNum == 2){
        foodNum = 3;
        s[1] = new snake(*this,2);
        Scene.addItem(s[1]);
        f[2] = new food(QPointF(450,150), 1);
        Scene.addItem(f[2]);
    }

    if(AI){
        foodNum = 3;
        ai = new AIsnake(*this,3);
        s[2] = ai;
        Scene.addItem(s[2]);
        f[2] = new food(QPointF(450,150), 1);
        Scene.addItem(f[2]);
    }

    s[0] = new snake(*this,1);
    Scene.addItem(s[0]);

    Scene.installEventFilter(this);

    start();
}

ControlCenter::~ControlCenter(){}

QPointF ControlCenter::randomPoint(){
    QPointF p;

    p.rx() = rand()*840/RAND_MAX;
    p.ry() = rand()*660/RAND_MAX;
    p.rx() = p.x() - (int)p.x() % 30 + 30;
    p.ry() = p.y() - (int)p.y() % 30 + 30;

    return p;
}

void ControlCenter::start(){
    connect(&timer,SIGNAL(timeout()),&Scene,SLOT(advance()));
  //  connect(&timer,SIGNAL(timeout()),&Scene,SLOT(SnakeColliding()));
}

void ControlCenter::pause(){
    disconnect(&timer,SIGNAL(timeout()),&Scene,SLOT(advance()));
    emit turnToPausePage();
}

void ControlCenter::GG(){
    disconnect(&timer,SIGNAL(timeout()),&Scene,SLOT(advance()));
    emit gameOver();
}

void ControlCenter::judgeWinner(int player){
   // disconnect(&timer,SIGNAL(timeout()),&Scene,SLOT(SnakeColliding()));
    disconnect(&timer,SIGNAL(timeout()),&Scene,SLOT(advance()));
    winner = player;
    emit win();
}

void ControlCenter::KeyPressed(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape){
        pause();
    }

    if(event->key() == Qt::Key_W)
        s[0]->redirection(UP);
    if(event->key() == Qt::Key_S)
        s[0]->redirection(DOWN);
    if(event->key() == Qt::Key_A)
        s[0]->redirection(LEFT);
    if(event->key() == Qt::Key_D)
        s[0]->redirection(RIGHT);

    if(playerNum == 2 && !ifAI){
        if(event->key() == Qt::Key_Up)
            s[1]->redirection(UP);
        if(event->key() == Qt::Key_Down)
            s[1]->redirection(DOWN);
        if(event->key() == Qt::Key_Left)
            s[1]->redirection(LEFT);
        if(event->key() == Qt::Key_Right)
            s[1]->redirection(RIGHT);
    }
}

void ControlCenter::SnakeIntoWall(int player){
   if(playerNum == 1){
        for(int i =0; i < wallNum; ++i){
            if(wall[i] == nullptr) continue;
            QPointF h;
            h.rx() = s[0]->head.x() + block;
            h.ry() = s[0]->head.y() + block;
            if(wall[i]->contains(h)&&wall[i]->contains(s[0]->head)){
                if(s[0]->inevitable){
                    Scene.removeItem(wall[i]);
                    wall[i]->setRect(1000,1000,1,1);
                }
                else {
                    if(--s[0]->life == 0){
                        if(!ifAI)
                            GG();
                        else
                            judgeWinner(3);
                    }
                    else
                        s[0]->Invincible2s();

                }
                return;
            }
        }
    }
    if(playerNum == 2){
        for(int i =0; i < wallNum; ++i){
            if(wall[i] == nullptr) continue;
            QPointF h;
            h.rx() = s[player-1]->head.x() + block;
            h.ry() = s[player-1]->head.y() + block;
            if(wall[i]->contains(s[player-1]->head)&&wall[i]->contains(h)){
                if(s[player-1]->inevitable){
                    Scene.removeItem(wall[i]);
                    wall[i]->setRect(1000,1000,1,1);
                }
                else {
                    if(--s[player-1]->life == 0){
                        if(player == 1)
                            judgeWinner(2);
                        else
                            judgeWinner(1);
                    }
                    else
                        s[player-1]->Invincible2s();
                }
                return;
            }
        }
    }
}

bool ControlCenter::handleCollisions(QPointF head, int player){
    for(int i = 0; i < foodNum; i++){
        if(f[i]!=nullptr&&f[i]->scenePos() == head){
            switch (f[i]->type) {
            case 1: case 5: s[player-1]->speedDown(); break;
            case 2: case 6: s[player-1]->speedUp(); break;
            case 3: s[player-1]->lifePlus(); break;
            case 4: s[player-1]->Inevitalbe2s();break;
            }
            addNewFood(f[i]);
            return true;
        }
    }
    return false;
}

void ControlCenter::automove(QPointF head, Direction &dir){
    QPointF target;
    double distance=0x7fffff;
    for(int i = 0; i < foodNum; i++){
        if(f[i] == nullptr) continue;
        QPointF tar = f[i]->scenePos();
        double dis = (tar.x()-head.x())*(tar.x()-head.x()) + (tar.y()-head.y())*(tar.y()-head.y());
        if(dis < distance){
            target = tar;
            distance = dis;
        }
    }

    if(!XDirection(head,target,dir)){
        if(!YDirection(head,target,dir)){
            if(!IntoItem(QPointF(head.x(),head.y()-block)) && dir - UP != 2){
                dir = UP;
                return;
            }
            if(!IntoItem(QPointF(head.x()+block,head.y())) && dir - RIGHT != 2) {
                dir = RIGHT;
                return;
            }
            if(!IntoItem(QPointF(head.x(),head.y()+block)) && DOWN - dir != 2) {
                dir = DOWN;
                return;
            }
            if(!IntoItem(QPointF(head.x()-block,head.y())) && LEFT - dir != 2) {
                dir = LEFT;
                return;
            }
        }
    }
}

bool ControlCenter::XDirection(QPointF head, QPointF target, Direction &dir){
    int deltaX = target.x() - head.x();
    if(deltaX == 0) return false;
    if(deltaX > 0) {
        if(!IntoItem(QPointF(head.x()+block,head.y())) && dir - RIGHT != 2) {
            dir = RIGHT;
            return true;
        }
        else
            return false;
    }
    if(deltaX < 0) {
        if(!IntoItem(QPointF(head.x()-block,head.y())) && LEFT - dir != 2) {
            dir = LEFT;
            return true;
        }
        else
            return false;
    }
      return false;
}

bool ControlCenter::YDirection(QPointF head, QPointF target, Direction &dir){
    int deltaY = target.y() - head.y();
    if(deltaY == 0) return false;
    if(deltaY > 0) {
        if(!IntoItem(QPointF(head.x(),head.y()+block)) && DOWN - dir != 2) {
            dir = DOWN;
            return true;
        }
        else
            return false;
    }
    if(deltaY < 0) {
        if(!IntoItem(QPointF(head.x(),head.y()-block)) && dir - UP != 2) {
            dir = UP;
            return true;
        }
        else
            return false;
    }
    return false;
}

bool ControlCenter::IntoItem(QPointF p){
    if(p.x() < 0 || p.x() >= 900 || p.y() < 0 || p.y() >= 720)
        return true;

    for(int i = 0; i < wallNum ; i++){
        if(wall[i]->contains(p)&&wall[i]->contains(QPointF(p.x()+block,p.y()+block)))
            return true;
    }

    if(s[0]->contains(p)&&s[0]->contains(QPointF(p.x()+block,p.y()+block))){
        return true;
    }

    if(ai->pathIntoBody(p))
        return true;
    return false;
}

void ControlCenter::SnakesColliding(){
    if(playerNum == 2) {
        if(s[0]->invincible||s[1]->invincible)
            return;

        if(s[0]->contains(s[1]->head)){
            if(s[1]->inevitable)
                judgeWinner(2);
            else
                judgeWinner(1);
        }
        else if(s[1]->contains(s[0]->head)){
            if(s[0]->inevitable)
                judgeWinner(1);
            else
                judgeWinner(2);
        }
    }
    if(ifAI) {
        if(s[0]->invincible||s[2]->invincible)
            return;

        if(s[0]->contains(s[2]->head)){
            if(s[2]->inevitable)
                judgeWinner(3);
            else
                judgeWinner(1);
        }
        else if(s[2]->contains(s[0]->head)){
            if(s[0]->inevitable)
                judgeWinner(1);
            else
                judgeWinner(3);
        }
    }
}

void ControlCenter::HeadIntoBody(int player){
    if(playerNum == 2) {
        if(player == 1) {
            if(--s[0]->life == 0)
                judgeWinner(2);
            else
                s[0]->Invincible2s();
        }
        if(player == 2) {
            if(--s[1]->life == 0)
                judgeWinner(1);
            else
                s[1]->Invincible2s();
        }
    }
    if(playerNum == 1 && !ifAI) {
        if(--s[0]->life == 0)
            GG();
        else
            s[0]->Invincible2s();
    }
    if(playerNum == 1 && ifAI) {
        if(player == 1) {
            if(--s[0]->life == 0)
                judgeWinner(3);
            else
                s[0]->Invincible2s();
        }
        if(player == 3) {
            if(--s[2]->life == 0)
                judgeWinner(1);
            else
                s[2]->Invincible2s();
        }
    }
}

void ControlCenter::addNewFood(food *f){
    QPointF p;
    bool flag;
    do{
        p = randomPoint();
        flag = s[0]->shape().contains(s[0]->mapFromScene(p));

        if(playerNum == 2)
            flag = flag|(s[1]->shape().contains(s[1]->mapFromScene(p)));

        if(ifAI)
            flag = flag|(s[2]->shape().contains(s[2]->mapFromScene(p)));

        for(int i= 0; i < wallNum ; i++){
            if(wall[i]->contains(p)){
                flag = true;
                break;
            }
        }
    } while(flag);

    f->setPos(p);
    f->changeType(rand()%20);
}

bool ControlCenter::eventFilter(QObject *object, QEvent *event){
    if(event->type() == QEvent::KeyPress){
        KeyPressed((QKeyEvent *)event);
        return true;
    }
    else
        return QObject::eventFilter(object, event);
}
