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
#include <QGraphicsSceneMouseEvent>
#include "bricks.h"

ControlCenter::ControlCenter(QGraphicsScene &scene, QObject *parent, int num, bool AI):
    QObject(parent),
    winner(1),
    foodNum(2),
    Scene(scene),
    ifAI(AI),
    playerNum(num),
    wallNum(5),
    editable(false),
    ifMoving(false)
{
    timer.start(15);
    srand((unsigned)time(NULL));

    cheats.push_back(UP);cheats.push_back(UP);
    cheats.push_back(DOWN);cheats.push_back(DOWN);
    cheats.push_back(LEFT);cheats.push_back(RIGHT);
    cheats.push_back(LEFT);cheats.push_back(RIGHT);

    originWall[0] = new Bricks(QPointF(180, 150), 2, 2);
    originWall[1] = new Bricks(QPointF(660, 150), 2, 2);
    originWall[2] = new Bricks(QPointF(180, 510), 2, 2);
    originWall[3] = new Bricks(QPointF(660, 510), 2, 2);
    originWall[4] = new Bricks(QPointF(420, 330), 2 ,2);
    for(int i = 0 ; i<wallNum; ++i){
        Wall.push_back(originWall[i]);
        Scene.addItem(originWall[i]);
    }

    originFood[0] = new food(QPointF(210,360));
    Scene.addItem(originFood[0]);
    Food.push_back(originFood[0]);
    originFood[1] = new food(QPointF(660,360));
    Scene.addItem(originFood[1]);
    Food.push_back(originFood[1]);

    if(playerNum == 2){
        foodNum = 3;
        s[1] = new snake(*this,2);
        Scene.addItem(s[1]);
        originFood[2] = new food(QPointF(450,150), 1);
        Scene.addItem(originFood[2]);
        Food.push_back(originFood[2]);
    }

    if(AI){
        foodNum = 3;
        ai = new AIsnake(*this,3);
        s[2] = ai;
        Scene.addItem(s[2]);
        originFood[2] = new food(QPointF(450,150), 1);
        Scene.addItem(originFood[2]);
        Food.push_back(originFood[2]);
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
    p.rx() = p.x() - (int)p.x() % (int)block + block;
    p.ry() = p.y() - (int)p.y() % (int)block + block;

    return p;
}

void ControlCenter::start(){
    connect(&timer,SIGNAL(timeout()),&Scene,SLOT(advance()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(sendUpdate()));
    editable = false;
    ifMoving = false;
}

void ControlCenter::sendUpdate(){
    emit updateStatus();
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
    disconnect(&timer,SIGNAL(timeout()),&Scene,SLOT(advance()));
    winner = player;
    emit win();
}

void ControlCenter::KeyPressed(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape){
        pause();
    }

    switch(event->key()){
    case Qt::Key_W: s[0]->moves.push_back(UP); s[0]->useCheat.push_back(UP); break;
    case Qt::Key_S: s[0]->moves.push_back(DOWN); s[0]->useCheat.push_back(DOWN); break;
    case Qt::Key_A: s[0]->moves.push_back(LEFT); s[0]->useCheat.push_back(LEFT); break;
    case Qt::Key_D: s[0]->moves.push_back(RIGHT); s[0]->useCheat.push_back(RIGHT); break;
    }

    if(playerNum == 2 && !ifAI){
        switch(event->key()){
        case Qt::Key_Up: s[1]->moves.push_back(UP); s[1]->useCheat.push_back(UP); break;
        case Qt::Key_Down: s[1]->moves.push_back(DOWN); s[1]->useCheat.push_back(DOWN); break;
        case Qt::Key_Left: s[1]->moves.push_back(LEFT); s[1]->useCheat.push_back(LEFT); break;
        case Qt::Key_Right: s[1]->moves.push_back(RIGHT); s[1]->useCheat.push_back(RIGHT); break;
        }
    }

    if(s[0]->useCheat.length() == 8 && !s[0]->render)
        checkCheats(s[0]->useCheat, 1);
    if(playerNum == 2&& s[1]->useCheat.length() == 8 && !s[1]->render)
        checkCheats(s[1]->useCheat, 2);
}

void ControlCenter::checkCheats(QVector<Direction> &directs, int player){
    for (int i = 0; i < 8; ++i) {
        if(cheats[i] != directs[i]){
            directs.pop_front();
            return;
        }
    }
    s[player - 1]->render = true;

    if(ifAI){
        ai->render = true;
    }
}

void ControlCenter::SnakeIntoWall(int player){
   if(playerNum == 1 && !ifAI){
        for(Bricks* w: Wall) {
            if(w == nullptr) continue;
            QPointF h;
            h.rx() = s[0]->head.x() + block;
            h.ry() = s[0]->head.y() + block;
            if(w->contains(h)&&w->contains(s[0]->head)){
                if(s[0]->inevitable){
                    Scene.removeItem(w);
                    Wall.removeOne(w);
                }
                else {
                    if(--s[0]->life == 0) {
                            GG();
                    }
                    else {
                        s[0]->Invincible2s();
                    }
                }
                return;
            }
        }
    }
    if(playerNum == 2 || ifAI) {
        for(Bricks* w: Wall){
            if(w == nullptr) continue;
            QPointF h;
            h.rx() = s[player-1]->head.x() + block;
            h.ry() = s[player-1]->head.y() + block;
            if(w->contains(s[player-1]->head)&&w->contains(h)){
                if(s[player-1]->inevitable){
                    Scene.removeItem(w);
                    Wall.removeOne(w);
                }
                else {
                    if(--s[player-1]->life == 0){
                        if(player == 1){
                            if(ifAI)
                                judgeWinner(3);
                            else
                                judgeWinner(2);
                        }
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

bool ControlCenter::EatFood(QPointF head, int player){
    for(food* f: Food){
        if(f!=nullptr&&f->scenePos() == head){
            switch (f->type) {
            case 1: case 5: s[player-1]->speedDown(); break;
            case 2: case 6: s[player-1]->speedUp(); break;
            case 3: s[player-1]->lifePlus(); break;
            case 4: s[player-1]->Inevitalbe2s();break;
            }
            addNewFood(f);
            return true;
        }
    }
    return false;
}

void ControlCenter::automove(QPointF head, Direction &dir){
    QPointF target;
    double distance=0x7fffff;
    for(food* f: Food){
        if(f == nullptr) continue;
        QPointF tar = f->scenePos();
        double dis = (tar.x()-head.x())*(tar.x()-head.x()) + (tar.y()-head.y())*(tar.y()-head.y());
        if(dis < distance){
            target = tar;
            distance = dis;
        }
    }

    if(ai->inevitable)
        target = s[0]->head;

    if(!XDirection(head,target,dir)){
        if(!YDirection(head,target,dir)){
            if(!IntoItem(QPointF(head.x()-block,head.y())) && LEFT - dir != 2) {
                dir = LEFT;
                return;
            }
            if(!IntoItem(QPointF(head.x(),head.y()+block)) && DOWN - dir != 2) {
                dir = DOWN;
                return;
            }
            if(!IntoItem(QPointF(head.x()+block,head.y())) && dir - RIGHT != 2) {
                dir = RIGHT;
                return;
            }
            if(!IntoItem(QPointF(head.x(),head.y()-block)) && dir - UP != 2){
                dir = UP;
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
    if(!ai->inevitable){
        for(Bricks* w: Wall){
            if (w->contains(p)&& w->contains(QPointF(p.x()+block,p.y()+block)))
                return true;
        }

        if(s[0]->contains(p)){
            return true;
        }
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
        flag = s[0]->contains(s[0]->mapFromScene(p));

        if(playerNum == 2)
            flag = flag|(s[1]->contains(s[1]->mapFromScene(p)));

        if(ifAI)
            flag = flag|(s[2]->contains(s[2]->mapFromScene(p)));

        for(Bricks* w: Wall) {
            if(w->contains(p)) {
                flag = true;
                break;
            }
        }
    } while(flag);

    f->setPos(p);
    f->changeType(rand()%20);
}

bool ControlCenter::eventFilter(QObject *object, QEvent *event){
    if(event->type() == QEvent::KeyPress && !editable){
        KeyPressed((QKeyEvent *)event);
        return true;
    } else if(event->type() == QEvent::GraphicsSceneMousePress && editable) {        
        mousePress((QGraphicsSceneMouseEvent*) event);
        return true;
    }
    else
        return QObject::eventFilter(object, event);
}

void ControlCenter::mousePress(QGraphicsSceneMouseEvent* e){
    if(e->button() == Qt::LeftButton) {
        if(!ifMoving) {
            for(QGraphicsItem* i: Scene.items()){
                if(i->acceptDrops()&&i->contains(e->scenePos())) {
                    movingItem = i;
                    connect(&timer,SIGNAL(timeout()),this,SLOT(blink()));
                    blinkCounter = 0;
                    ifMoving = true;
                    break;
                }
            }
        }
        else if(ifMoving && movingItem != nullptr) {
            disconnect(&timer,SIGNAL(timeout()),this,SLOT(blink()));
            QPointF p = e->scenePos();
            movingItem->setPos(QPointF(p.x()-(int)p.x()%30, p.y()-(int)p.y()%30));
            if(!Scene.items().contains(movingItem))
                Scene.addItem(movingItem);
            ifMoving = false;
        }
    }

    if(e->button() == Qt::RightButton) {
        for(QGraphicsItem* i: Scene.items()){
            if(i->acceptDrops()&&i->contains(e->scenePos())){
                Scene.removeItem(i);
                i->setPos(1000,1000); //avoid collision
                disconnect(&timer,SIGNAL(timeout()),this,SLOT(blink()));
                if(movingItem!=nullptr && !Scene.items().contains(movingItem))
                    Scene.addItem(movingItem);
                ifMoving = false;
                break;
            }
        }
    }
}

void ControlCenter::blink(){
    if(blinkCounter%15 == 7) {
        Scene.addItem(movingItem);
    }
    if(blinkCounter%15 == 0) {
        Scene.removeItem(movingItem);
    }
    blinkCounter++;
}

void ControlCenter::newFood(){
    food* newFood = new food(QPointF(0,0));
    Scene.addItem(newFood);
    this->addNewFood(newFood);
    Food.push_back(newFood);
}

void ControlCenter::newWall(){
    Bricks* newWall = new Bricks(QPointF(450,0),1,1);
    Scene.addItem(newWall);
    Wall.push_back(newWall);
}
