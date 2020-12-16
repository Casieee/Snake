#ifndef SUBWIDGET_H
#define SUBWIDGET_H
#include <QWidget>
#include <QPushButton>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "controlcenter.h"
#include "pausepage.h"
#include "ggpage.h"
#include "winnerpage.h"

class subWidget: public QWidget
{
    Q_OBJECT

public:
    subWidget(int num = 1, bool AI = false, QWidget *parent=nullptr);

public slots:
    void backToMenu(); 

signals:
    void closeMe();
    void startNewGame();

private:
    int playernum;
    PausePage *pauseP;
    GGPage *gg;
    WinnerPage *win;
    ControlCenter *game;
    QGraphicsScene *scene;
    QGraphicsView *view;
};

#endif // SUBWIDGET_H
