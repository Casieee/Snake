#ifndef PAUSEPAGE_H
#define PAUSEPAGE_H

#include <QWidget>

class PausePage: public QWidget
{
    Q_OBJECT
public:
    PausePage(QWidget *parent = nullptr);
    void send();
    void ToMenu();

signals:
    void resume();
    void back();
    void saveFile();
    void loadFile();
    void editMode();
};

#endif // PAUSEPAGE_H
