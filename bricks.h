#ifndef BRICKS_H
#define BRICKS_H

#include <QObject>
#include <QGraphicsItem>

class Bricks : public QGraphicsItem
{
public:
    Bricks(QPointF P, int length = 0, int width = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QRectF boundingRect() const;
    bool contains(const QPointF &t) const;

private:
    int length;
    int width;
};

#endif // BRICKS_H
