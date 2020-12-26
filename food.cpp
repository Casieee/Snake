#include "food.h"
#include <QPainter>
#include "constants.h"

food::food(QPointF P, int type):
    type(0),
    color(NavajoWhite)
{
    setPos(P);
    changeType(type);
    setAcceptDrops(true);
}

QRectF food::boundingRect() const
{
    return QRectF(0, 0, block, block );
}

void food::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(), color);

    painter->restore();
}

QPainterPath food::shape() const
{
    QPainterPath p;
    p.addEllipse(QPointF(block/2,block/2), block/2-5, block/2-5);
    return p;
}

bool food::contains(const QPointF &t) const{
    QPointF tl = this->scenePos();
    QRectF r(tl.x(),tl.y(),block,block);
    if(r.contains(t))
        return true;
    else
        return false;
}

void food::changeType(int t){
    type = t;
    switch (type) {
    case 1: case 5: case 8: color = skyblue; break;
    case 2: case 6: case 9: color = blue2; break;
    case 3:  color = green; break;
    case 4: color = darkred; break;
    case 7: case 10: color = hotPink; break;
    default: color = NavajoWhite;
    }
}
