#include "winnerpage.h"
#include <QLabel>
#include <QPushButton>
#include <QDebug>

WinnerPage::WinnerPage(int winner, QWidget *parent):
    QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedSize(800,600);
    setAttribute(Qt::WA_DeleteOnClose);
    switch (winner) {
    case 1:
        this->setStyleSheet("background-color:lightblue"); break;
    case 2:
        this->setStyleSheet("background-color:lightpink"); break;
    case 3:
        this->setStyleSheet("background-color:rgb(0,0,0)"); break;
    }
    setWindowIcon(QIcon(":/snakeIcon/snake.png"));

    if(winner == 3){
        QLabel *title = new QLabel(this);
        title->setText("YOU ARE DEFEATED BY AI");
        title->setFixedSize(900,300);
        title->move(-50,100);
        title->setAlignment(Qt::AlignTop);title->setAlignment(Qt::AlignHCenter);
        title->setStyleSheet("QLabel{font-family:'cascadia code';font-size:55px;color:rgb(254,254,254);}");
        title->show();
    }
    else {
        QLabel *title = new QLabel(this);
        title->setText("The Winner is Player");
        title->setFixedSize(600,300);
        title->move(30,100);
        title->setAlignment(Qt::AlignTop);title->setAlignment(Qt::AlignHCenter);
        title->setStyleSheet("QLabel{font-family:'Showcard Gothic';font-size:50px;color:rgb(255,250,250);}");
        title->show();

        QLabel *num = new QLabel(this);
        if(winner == 1){
            num->setText("1");
            num->setStyleSheet("QLabel{font-family:'Forte';font-size:150px;color:rgb(0,0,128);}");
        }
        else{
            num->setText("2");
            num->setStyleSheet("QLabel{font-family:'Forte';font-size:150px;color:rgb(255,99,71);}");
        }
        num->setFixedSize(200,400);
        num->move(655,50);
        num->setAlignment(Qt::AlignTop);num->setAlignment(Qt::AlignLeft);
        num->show();
    }

    QPushButton *exit = new QPushButton(this);
    exit->setText("BACK");
    exit->setFixedSize(200,60);
    exit->move(300,460);
    exit->setStyleSheet("QPushButton{font-family:'Bauhaus 93';font-size:28px;color:rgb(0,0,0);}\
                        QPushButton{background-color:rgb(255,250,250)}\
                        QPushButton:hover{background-color:rgb(255, 222, 173)}");
    exit->show();

    QPushButton *restart = new QPushButton(this);
    restart->setText("RESTART");
    restart->setFixedSize(200,60);
    restart->move(300,360);
    restart->setStyleSheet("QPushButton{font-family:'Bauhaus 93';font-size:28px;color:rgb(0,0,0);}\
                          QPushButton{background-color:rgb(255,250,250)}\
                          QPushButton:hover{background-color:rgb(255, 222, 173)}");
    restart->show();

    connect(exit,&QPushButton::clicked,this,[=](){
        this->close();
       emit Back();
    });
    connect(restart,&QPushButton::clicked,this,[=](){
        this->close();
        emit Restart();
    });
}
