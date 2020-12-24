#include "bricks.h"
#include "constants.h"
#include <QPainter>

Bricks::Bricks(QPointF P, int length, int width):
    length(length),
    width(width)
{
    setPos(P);
    setAcceptDrops(true);
}

void Bricks::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    painter->save();

    painter->setBrush(Qt::black);
    painter->drawRect(QRectF(0,0,length*block,width*block));

    painter->restore();
}

QRectF Bricks::boundingRect() const{
    return QRectF(0,0,length*block,width*block);
}

bool Bricks::contains(const QPointF &t) const{
    QPointF tl = this->scenePos();
    QRectF r(tl.x(),tl.y(),length*block,width*block);
    if(r.contains(t))
        return true;
    else
        return false;
}
