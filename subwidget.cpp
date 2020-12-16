#include "subwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include "constants.h"
#include "food.h"
#include <QDebug>

subWidget::subWidget(int num, bool AI, QWidget *parent):
    QWidget(parent),
    playernum(num),
    scene(new QGraphicsScene(this)),
    view(new QGraphicsView(scene,this))
{
    this->setFixedSize(900,720);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);
    setAttribute(Qt::WA_DeleteOnClose);

    scene->setSceneRect(0,0,900,720);

    if(playernum == 2)
        game = new ControlCenter(*scene,this,2);
    else
        game = new ControlCenter(*scene,this,1,AI);

    //pausePage
    connect(game,&ControlCenter::turnToPausePage,this,[=](){
        pauseP = new PausePage;
        pauseP->show();
        connect(pauseP,&PausePage::resume,game,&ControlCenter::start);
        connect(pauseP,&PausePage::back,this,&subWidget::backToMenu);
    });

    //ggPage
    if(playernum == 1 && !AI)
        connect(game,&ControlCenter::gameOver,this,[=](){
            qDebug() << "gg";
            gg = new GGPage;
            gg->show();
            connect(gg,&GGPage::Back,this,&subWidget::backToMenu);
            connect(gg,&GGPage::Restart,this,[=](){
                this->close();
                emit startNewGame();
            });
        });

    //winnerPage
    if(playernum == 2 || AI)
        connect(game,&ControlCenter::win,this,[=](){
            win = new WinnerPage(game->winner);
            win->show();
            connect(win,&WinnerPage::Back,this,&subWidget::backToMenu);
            connect(win,&WinnerPage::Restart,this,[=](){
                this->close();
                emit startNewGame();
            });
        });
}

void subWidget::backToMenu(){
    scene->clear();
    this->close();
    emit closeMe();
}
