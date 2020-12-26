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
    bool contains(const QPointF &t) const;

    int type;
    QColor color;
    void changeType(int t);
};

#endif // FOOD_H
