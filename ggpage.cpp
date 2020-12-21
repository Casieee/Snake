#include "ggpage.h"
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>

GGPage::GGPage(QWidget *parent):
    QWidget(parent)
{
    this->setFixedSize(1100,720);
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);    
    this->setStyleSheet("background-color:rgb(0,0,0)");
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon(":/snakeIcon/snake.png"));

    QLabel *gg = new QLabel(this);
    gg->setText("GAME OVER");
    gg->setFixedSize(1100,360);
    gg->move(0,100);
    gg->setAlignment(Qt::AlignTop);gg->setAlignment(Qt::AlignHCenter);
    gg->setStyleSheet("QLabel{font-family:'OCR A Extended';font-size:150px;color:rgb(255,250,250);}");
    gg->show();

    QPushButton *exit = new QPushButton(this);
    exit->setText("BACK");
    exit->setFixedSize(300,90);
    exit->move(400,560);
    exit->setStyleSheet("QPushButton{font-family:'Bauhaus 93';font-size:50px;color:rgb(0,0,0);}\
                        QPushButton{background-color:rgb(255,250,250)}\
                        QPushButton:hover{background-color:rgb(255, 222, 173)}");
    exit->show();

    QPushButton *restart = new QPushButton(this);
    restart->setText("RESTART");
    restart->setFixedSize(300,90);
    restart->move(400,400);
    restart->setStyleSheet("QPushButton{font-family:'Bauhaus 93';font-size:50px;color:rgb(0,0,0);}\
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
