#include "winnerpage.h"
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QFont>`

WinnerPage::WinnerPage(int winner, QWidget *parent):
    QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedSize(1100,720);
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);
    setAttribute(Qt::WA_DeleteOnClose);

    switch (winner) {
    case 1:
        this->setStyleSheet("background-color:lightblue"); break;
    case 2:
        this->setStyleSheet("background-color:lightpink"); break;
    case 3:
        this->setStyleSheet("background-color:rgb(0,0,0)"); break;
    case 4:
        this->setStyleSheet("background-color:rgb(255,255,255)"); break;
    }
    setWindowIcon(QIcon(":/snakeIcon/snake.png"));

    if(winner == 3 || winner == 4){
        QLabel *title = new QLabel(this);
        title->move(0,100);

        if(winner == 3) {
            title->setFixedSize(1100,360);
            title->setText("YOU ARE DEFEATED BY AI");
            title->setAlignment(Qt::AlignTop);title->setAlignment(Qt::AlignHCenter);
            title->setStyleSheet("QLabel{font-family:'cascadia code';font-size:70px;color:rgb(254,254,254);}");
        }
        else {
            title->setFixedSize(700,150);
            title->setText("You Stand Up For");
            title->setAlignment(Qt::AlignTop);title->setAlignment(Qt::AlignHCenter);
            title->setStyleSheet("QLabel{font-family:'Centaur';font-size:65px;color:rgb(233,168,63);}");

            QLabel* titlePlus = new QLabel(this);
            titlePlus->setFixedSize(900,220);
            titlePlus->move(110,200);
            titlePlus->setText("HUMAN DIGITY");
            titlePlus->setAlignment(Qt::AlignTop);titlePlus->setAlignment(Qt::AlignRight);
            titlePlus->setStyleSheet("QLabel{font-famliy:'Centaur';"
                                     "font-size:80px;"
                                     "color:rgb(100,3,197);}");
        }
        title->show();
    }
    else {
        QLabel *title = new QLabel(this);
        title->setText("The Winner is Player");
        title->setFixedSize(800,360);
        title->move(50,120);
        title->setAlignment(Qt::AlignTop);title->setAlignment(Qt::AlignHCenter);
        title->setStyleSheet("QLabel{font-family:'Showcard Gothic';font-size:70px;color:rgb(255,250,250);}");
        title->show();

        QLabel *num = new QLabel(this);
        if(winner == 1){
            num->setText("1");
            num->setStyleSheet("QLabel{font-family:'Forte';font-size:200px;color:rgb(0,0,128);}");
        }
        else{
            num->setText("2");
            num->setStyleSheet("QLabel{font-family:'Forte';font-size:200px;color:rgb(255,99,71);}");
        }
        num->setFixedSize(200,400);
        num->move(900,50);
        num->setAlignment(Qt::AlignTop);num->setAlignment(Qt::AlignLeft);
        num->show();
    }

    QPushButton *exit = new QPushButton(this);
    exit->setText("BACK");
    exit->setFixedSize(300,90);
    exit->move(400,560);
    exit->setStyleSheet("QPushButton{font-family:'Bauhaus 93';font-size:50px;color:rgb(0,0,0);}\
                        QPushButton{background-color:rgb(255,250,250)}\
                        QPushButton:hover{background-color:rgb(39,64,139)}");
    exit->show();

    QPushButton *restart = new QPushButton(this);
    restart->setText("RESTART");
    restart->setFixedSize(300,90);
    restart->move(400,400);
    restart->setStyleSheet("QPushButton{font-family:'Bauhaus 93';font-size:50px;color:rgb(0,0,0);}\
                          QPushButton{background-color:rgb(255,250,250)}\
                          QPushButton:hover{background-color:rgb(39,64,139)}");
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
