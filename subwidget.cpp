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
    scene->mouseGrabberItem();
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
        connect(pauseP,&PausePage::editMode,this,&subWidget::edit);
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
            if (AI && game->winner == 1){
                win = new WinnerPage(4);
            }
            else
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
            + "\n\n  Score: " + QString::number(game->s[0]->score)
            + "\n\n  Fuel: " + QString::number(game->s[0]->jetFuel));
    player1Status->setStyleSheet("QLabel{background-color:rgb(141,238,238);"
                                 "font-family:'Forte';font-size:30px;color:rgb(0,0,139);}");
    player1Status->setAlignment(Qt::AlignCenter);
    player1Status->show();
    connect(game, &ControlCenter::updateStatus, this, [=](){
        player1Status->setText(" Player 1\n\n Speed: " + QString::number(game->s[0]->speed)
                + "\n\n Life: " + QString::number(game->s[0]->life)
                + "\n\n Score: " + QString::number(game->s[0]->score)
                + "\n\n  Fuel: " + QString::number(game->s[0]->jetFuel));
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
                + "\n\n Score: " + QString::number(game->s[1]->score)
                + "\n\n  Fuel: " + QString::number(game->s[1]->jetFuel));
        player2Status->setStyleSheet("QLabel{background-color:rgb(255,193,193);"
                                     "font-family:'Forte';font-size:30px;color:rgb(255,48,48);}");
        player2Status->setAlignment(Qt::AlignCenter);
        player2Status->show();

        connect(game, &ControlCenter::updateStatus, this, [=](){
            player2Status->setText(" Player 2\n\n Speed: " + QString::number(game->s[1]->speed)
                    + "\n\n Life: " + QString::number(game->s[1]->life)
                    + "\n\n Score: " + QString::number(game->s[1]->score)
                    + "\n\n  Fuel: " + QString::number(game->s[1]->jetFuel));
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

void subWidget::edit(){
    game->editable = true;

    QLabel* cover = new QLabel(this);
    cover->setFixedSize(200,720);
    cover->move(900,0);
    cover->setStyleSheet("QLabel{background-color:NavajoWhite;}");
    cover->show();

    QLabel *S = new QLabel(this);
    S->setText("S");
    S->setFixedSize(200,180);
    S->move(900,0);
    S->setAlignment(Qt::AlignTop);S->setAlignment(Qt::AlignHCenter);
    S->setStyleSheet("QLabel{font-family:'Gigi';font-size:150px;color:rgb(46,139,87);}");
    S->show();

    QPushButton* addWall = new QPushButton(this);
    addWall->setFixedSize(150,60);
    addWall->setText("WALL");
    addWall->move(925,270);
    addWall->setStyleSheet("QPushButton{font-family:'Century';font-size:35px;color:rgb(0,0,0);}\
                           QPushButton{background-color:rgb(255,218,185)}\
                           QPushButton:hover{background-color:NavajoWhite}");
    addWall->show();

    QPushButton* addFood = new QPushButton(this);
    addFood->setFixedSize(150,60);
    addFood->setText("FOOD");
    addFood->move(925,420);
    addFood->setStyleSheet("QPushButton{font-family:'Century';font-size:35px;color:rgb(0,0,0);}\
                           QPushButton{background-color:rgb(255,218,185)}\
                           QPushButton:hover{background-color:NavajoWhite}");
    addFood->show();

    QPushButton* Save_Continue = new QPushButton(this);
    Save_Continue->setFixedSize(150,60);
    Save_Continue->setText("Continue");
    Save_Continue->move(925,570);
    Save_Continue->setStyleSheet("QPushButton{font-family:'Century';font-size:30px;color:rgb(0,0,0);}\
                           QPushButton{background-color:rgb(255,218,185)}\
                           QPushButton:hover{background-color:NavajoWhite}");
    Save_Continue->show();

    connect(addWall,&QPushButton::clicked,game,&ControlCenter::newWall);
    connect(addFood,&QPushButton::clicked,game,&ControlCenter::newFood);
    connect(Save_Continue,&QPushButton::clicked,this,[=](){
        cover->hide();
        S->hide();
        addWall->hide();
        addFood->hide();
        Save_Continue->hide();
        game->start();
    });
}
