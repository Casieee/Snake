#include "ggpage.h"
#include <QLabel>
#include <QPushButton>

GGPage::GGPage(QWidget *parent):
    QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedSize(800,600);
    this->setStyleSheet("background-color:rgb(0,0,0)");
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon(":/snakeIcon/snake.png"));

    QLabel *gg = new QLabel(this);
    gg->setText("GAME OVER");
    gg->setFixedSize(800,300);
    gg->move(0,100);
    gg->setAlignment(Qt::AlignTop);gg->setAlignment(Qt::AlignHCenter);
    gg->setStyleSheet("QLabel{font-family:'OCR A Extended';font-size:120px;color:rgb(255,250,250);}");
    gg->show();

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
