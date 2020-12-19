#include "subwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include "constants.h"
#include "food.h"
#include <QDebug>
#include "snake.h"
#include <QLabel>
#include "aisnake.h"

subWidget::subWidget(int num, bool AI, QWidget *parent):
    QWidget(parent),
    playernum(num),
    scene(new QGraphicsScene(this)),
    view(new QGraphicsView(scene,this))
{
    this->setFixedSize(1100,720);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon(":/snakeIcon/snake.png"));

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

    QLabel *player1Status = new QLabel(this);
    player1Status->setFixedSize(200,360);
    player1Status->move(900,0);
    player1Status->setText("  Player 1\n\n  Speed: " + QString::number(game->s[0]->speed)
            + "\n\n  Life: " + QString::number(game->s[0]->life)
            + "\n\n  Score: " + QString::number(game->s[0]->score));
    player1Status->setStyleSheet("QLabel{background-color:rgb(141,238,238);"
                                 "font-family:'Forte';font-size:30px;color:rgb(0,0,139);}");
    player1Status->setAlignment(Qt::AlignCenter);
    player1Status->show();
    connect(game, &ControlCenter::updateStatus, this, [=](){
        player1Status->setText(" Player 1\n\n Speed: " + QString::number(game->s[0]->speed)
                + "\n\n Life: " + QString::number(game->s[0]->life)
                + "\n\n Score: " + QString::number(game->s[0]->score));
    });

    QLabel *fillempty = new QLabel(this);
    fillempty->setFixedSize(200,360);
    fillempty->move(900,360);
    fillempty->setStyleSheet("QLabel{background-color:rgb(141,238,238);}");

    if(playernum == 2){
        QLabel *player2Status = new QLabel(this);
        player2Status->setFixedSize(200,360);
        player2Status->move(900,360);
        player2Status->setText(" Player 2\n\n Speed: " + QString::number(game->s[1]->speed)
                + "\n\n Life: " + QString::number(game->s[1]->life)
                + "\n\n Score: " + QString::number(game->s[1]->score));
        player2Status->setStyleSheet("QLabel{background-color:rgb(255,193,193);"
                                     "font-family:'Forte';font-size:30px;color:rgb(255,48,48);}");
        player2Status->setAlignment(Qt::AlignCenter);
        player2Status->show();

        connect(game, &ControlCenter::updateStatus, this, [=](){
            player2Status->setText(" Player 2\n\n Speed: " + QString::number(game->s[1]->speed)
                    + "\n\n Life: " + QString::number(game->s[1]->life)
                    + "\n\n Score: " + QString::number(game->s[1]->score));
        });
    }

    if(AI){
        QLabel *AIStatus = new QLabel(this);
        AIStatus->setFixedSize(200,360);
        AIStatus->move(900,360);
        AIStatus->setText(" AI\n\n Speed: " + QString::number(game->ai->speed)
                + "\n\n Life: " + QString::number(game->ai->life)
                + "\n\n Score: " + QString::number(game->ai->score));
        AIStatus->setStyleSheet("QLabel{background-color:rgb(0,0,0);"
                                "font-family:'Forte';font-size:30px;color:rgb(255,255,255);}");
        AIStatus->setAlignment(Qt::AlignCenter);
        AIStatus->show();

        connect(game, &ControlCenter::updateStatus, this, [=](){
            AIStatus->setText(" AI\n\n Speed: " + QString::number(game->ai->speed)
                    + "\n\n Life: " + QString::number(game->ai->life)
                    + "\n\n Score: " + QString::number(game->ai->score));
        });
    }
}

void subWidget::backToMenu(){
    scene->clear();
    this->close();
    emit closeMe();
}
