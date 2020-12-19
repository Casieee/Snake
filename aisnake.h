#ifndef AISNAKE_H
#define AISNAKE_H
#include "snake.h"

class AIsnake: public snake
{
public:
    AIsnake(ControlCenter &Game, int player);
    bool pathIntoBody(QPointF p);
    void advance(int phase);
};

#endif // AISNAKE_H
