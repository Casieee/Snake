#ifndef MODECHOOSEPAGE_H
#define MODECHOOSEPAGE_H

#include <QObject>
#include <QWidget>
#include "subwidget.h"

class ModeChoosePage: public QWidget
{
    Q_OBJECT
public:
    ModeChoosePage(QWidget *parent = nullptr);

private:
    subWidget *sub;
    void startNormal(subWidget *&sub);
    void startTwoPlayer(subWidget *&sub);
    void startAI(subWidget *&sub);

signals:
    void back();
};

#endif // MODECHOOSEPAGE_H
