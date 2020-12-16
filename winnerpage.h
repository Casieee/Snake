#ifndef WINNERPAGE_H
#define WINNERPAGE_H

#include <QObject>
#include <QWidget>

class WinnerPage: public QWidget
{
    Q_OBJECT
public:
    WinnerPage(int winner = 1, QWidget *parent = nullptr);

signals:
    void Restart();
    void Back();
};

#endif // WINNERPAGE_H
