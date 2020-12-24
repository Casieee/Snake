#include "pausepage.h"
#include <QPushButton>
#include <QLabel>

PausePage::PausePage(QWidget *parent):
    QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedSize(400,360);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon(":/snakeIcon/snake.png"));

    QLabel *S = new QLabel(this);
    S->setText("S");
    S->setFixedSize(200,200);
    S->move(100,0);
    S->setAlignment(Qt::AlignTop);S->setAlignment(Qt::AlignHCenter);
    S->setStyleSheet("QLabel{font-family:'Gigi';font-size:120px;color:rgb(46,139,87);}");
    S->show();

    QPushButton *exit = new QPushButton(this);
    exit->setText("Back");
    exit->setFixedSize(200,30);
    exit->move(100,310);
    exit->setStyleSheet("QPushButton{font-family:'Bauhaus 93';font-size:24px;color:rgb(255,250,250);}\
                        QPushButton{background-color:rgb(170,200,50)}\
                        QPushButton:hover{background-color:rgb(50, 170, 200)}");
    exit->show();

    QPushButton *edit = new QPushButton(this);
    edit->setText("Edit");
    edit->setFixedSize(200,30);
    edit->move(100,270);
    edit->setStyleSheet("QPushButton{font-family:'Bauhaus 93';font-size:24px;color:rgb(255,250,250);}\
                        QPushButton{background-color:rgb(170,200,50)}\
                        QPushButton:hover{background-color:rgb(50, 170, 200)}");
    edit->show();

    QPushButton *resume = new QPushButton(this);
    resume->setText("Resume");
    resume->setFixedSize(200,30);
    resume->move(100,150);
    resume->setStyleSheet("QPushButton{font-family:'Bauhaus 93';font-size:24px;color:rgb(255,250,250);}\
                          QPushButton{background-color:rgb(170,200,50)}\
                          QPushButton:hover{background-color:rgb(50, 170, 200)}");
    resume->show();

    QPushButton *save = new QPushButton(this);
    save->setText("Save");
    save->setFixedSize(200,30);
    save->move(100,190);
    save->setStyleSheet("QPushButton{font-family:'Bauhaus 93';font-size:24px;color:rgb(255,250,250);}\
                          QPushButton{background-color:rgb(170,200,50)}\
                          QPushButton:hover{background-color:rgb(50, 170, 200)}");
    save->show();

    QPushButton *load = new QPushButton(this);
    load->setText("Load");
    load->setFixedSize(200,30);
    load->move(100,230);
    load->setStyleSheet("QPushButton{font-family:'Bauhaus 93';font-size:24px;color:rgb(255,250,250);}\
                          QPushButton{background-color:rgb(170,200,50)}\
                          QPushButton:hover{background-color:rgb(50, 170, 200)}");
    load->show();

    connect(resume,&QPushButton::clicked,this,&PausePage::send);
    connect(save,&QPushButton::clicked,this,[=](){
        emit saveFile();
    });
    connect(load,&QPushButton::clicked,this,[=](){
        this->close();
        emit loadFile();
    });
    connect(exit,&QPushButton::clicked,this,&PausePage::ToMenu);
    connect(edit,&QPushButton::clicked,this,[=](){
        this->close();
        emit editMode();
    });
}

void PausePage::send(){
    this->close();
    emit resume();
}

void PausePage::ToMenu(){
    this->close();
    emit back();
}
