#include "modechoosepage.h"
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QLabel>
#include <QDebug>

ModeChoosePage::ModeChoosePage(QWidget *parent):
    QWidget(parent)
{
    this->setFixedSize(900,720);
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);
    this->setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon(":/snakeIcon/snake.png"));

    QLabel *title = new QLabel(this);
    title->setText("S");
    title->setFixedSize(600,300);
    title->move(150,50);
    title->setAlignment(Qt::AlignTop);title->setAlignment(Qt::AlignHCenter);
    title->setStyleSheet("QLabel{font-family:'Gigi';font-size:200px;color:rgb(46,139,87);}");
    title->show();

    QPushButton *Single = new QPushButton;
    Single->setText("Single");
    Single->setParent(this);
    Single->setFixedSize(350,60);
    Single->move(275,360);
    Single->setStyleSheet(
                "QPushButton{font-family:'Bauhaus 93';font-size:32px;color:rgb(255,250,250);}\
                 QPushButton{background-color:rgb(170,200,50)}\
                 QPushButton:hover{background-color:rgb(50, 170, 200)}");
    Single->show();

    QPushButton *PvP = new QPushButton;
    PvP->setText("Player VS Player");
    PvP->setParent(this);
    PvP->setFixedSize(350,60);
    PvP->move(275,440);
    PvP->setStyleSheet(
                "QPushButton{font-family:'Bauhaus 93';font-size:32px;color:rgb(255,250,250);}\
                 QPushButton{background-color:rgb(170,200,50)}\
                 QPushButton:hover{background-color:rgb(50, 170, 200)}");
    PvP->show();

    QPushButton *AI = new QPushButton("Player VS AI",this);
    AI->setFixedSize(350,60);
    AI->move(275,520);
    AI->setStyleSheet(
                "QPushButton{font-family:'Bauhaus 93';font-size:32px;color:rgb(255,250,250);}\
                 QPushButton{background-color:rgb(170,200,50)}\
                 QPushButton:hover{background-color:rgb(50, 170, 200)}");
    AI->show();

    QPushButton *ExitButton = new QPushButton("Back",this);
    ExitButton->setFixedSize(350,60);
    ExitButton->move(275,600);
    ExitButton->setStyleSheet(
                "QPushButton{font-family:'Bauhaus 93';font-size:32px;color:rgb(255,250,250);}\
                 QPushButton{background-color:rgb(170,200,50)}\
                 QPushButton:hover{background-color:rgb(50, 170, 200)}");
    ExitButton->show();


    connect(Single,&QPushButton::clicked,this,[=](){
        sub = new subWidget;
        this->hide();
        sub->show();
        startNormal(sub);
    });

    connect(PvP,&QPushButton::clicked,this,[=](){
        sub = new subWidget(2);
        this->hide();
        sub->show();
        startTwoPlayer(sub);
    });

    connect(AI,&QPushButton::clicked,this,[=](){
        sub = new subWidget(1,true);
        this->hide();
        sub->show();
        startAI(sub);
    });

    connect(ExitButton,&QPushButton::clicked,this,[=](){
        this->close();
        emit back();
    });
}

void ModeChoosePage::startNormal(subWidget* &sub){
    if(sub!=nullptr){
        connect(sub,&subWidget::closeMe,this,&QWidget::show);
        connect(sub,&subWidget::startNewGame,this,[=,&sub](){
            delete sub;
            sub = new subWidget(1);
            startNormal(sub);
            sub->show();
        });
    }
}

void ModeChoosePage::startTwoPlayer(subWidget* &sub){
    if(sub!=nullptr){
        connect(sub,&subWidget::closeMe,this,&QWidget::show);
        connect(sub,&subWidget::startNewGame,this,[=,&sub](){
            delete sub;
            sub = new subWidget(2);
            startTwoPlayer(sub);
            sub->show();
        });
    }
}

void ModeChoosePage::startAI(subWidget* &sub){
    if(sub!=nullptr){
        connect(sub,&subWidget::closeMe,this,&QWidget::show);
        connect(sub,&subWidget::startNewGame,this,[=,&sub](){
            delete sub;
            sub = new subWidget(1,true);
            startAI(sub);
            sub->show();
        });
    }
}
