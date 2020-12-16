#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "subwidget.h"
#include "modechoosepage.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    subWidget* sub;
    ModeChoosePage* mode;
};
#endif // WIDGET_H
