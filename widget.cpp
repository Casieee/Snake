#include "widget.h"
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QLabel>
#include <QFont>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(900,720);
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);
    this->setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/snakeIcon/snake.png"));

    QPushButton *PlayButton = new QPushButton;
    PlayButton->setText("PLAY");
    PlayButton->setParent(this);
    PlayButton->setFixedSize(250,80);
    PlayButton->move(300,400);
    PlayButton->setStyleSheet(
                "QPushButton{font-family:'Bauhaus 93';font-size:32px;color:rgb(255,250,250);}\
                 QPushButton{background-color:rgb(170,200,50)}\
                 QPushButton:hover{background-color:rgb(50, 170, 200)}");
    PlayButton->show();

    QPushButton *ExitButton = new QPushButton("EXIT",this);
    ExitButton->setFixedSize(250,80);
    ExitButton->move(300,540);
    ExitButton->setStyleSheet(
                "QPushButton{font-family:'Bauhaus 93';font-size:32px;color:rgb(255,250,250);}\
                 QPushButton{background-color:rgb(170,200,50)}\
                 QPushButton:hover{background-color:rgb(50, 170, 200)}");
    ExitButton->show();
    connect(ExitButton,&QPushButton::clicked,this,&QWidget::close);

    QLabel *title = new QLabel(this);
    title->setText("SNAKE");
    title->setFixedSize(600,200);
    title->move(150,150);
    title->setAlignment(Qt::AlignTop);title->setAlignment(Qt::AlignHCenter);
    title->setStyleSheet("QLabel{font-family:'Gigi';font-size:120px;color:rgb(60,179,113);}");
    title->show();

    connect(PlayButton,&QPushButton::clicked,this,[=](){
        mode = new ModeChoosePage;
        this->hide();
        mode->show();
        connect(mode,&ModeChoosePage::back,this,&QWidget::show);
    });        
}

Widget::~Widget()
{
}
