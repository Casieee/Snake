#ifndef FOOD_H
#define FOOD_H
#include <QGraphicsItem>
#include "constants.h"
class food: public QGraphicsItem
{
public:
    food(QPointF P,int type = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QPainterPath shape() const;

    //type == 0 -> normal , type == 1 -> speed up
    //type == 2 -> speed down , type == 3 -> life++
    int type;
    QColor color;
    void changeType(int t);
};

#endif // FOOD_H
