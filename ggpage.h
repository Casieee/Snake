#ifndef GGPAGE_H
#define GGPAGE_H

#include <QObject>
#include <QWidget>

class GGPage: public QWidget
{
    Q_OBJECT
public:
    GGPage(QWidget *parent=nullptr);

signals:
    void Restart();
    void Back();
};

#endif // GGPAGE_H
