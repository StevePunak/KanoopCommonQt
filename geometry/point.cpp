#include "point.h"

using namespace Geo;

void Point::move(Direction direction, double amount)
{
    switch(direction) {
    case Up:
        setY(y() - amount);
        break;
    case Down:
        setY(y() + amount);
        break;
    case ToLeft:
        setX(x() - amount);
        break;
    case ToRight:
        setX(x() + amount);
        break;
    default:
        break;
    }
}

Point &Point::round()
{
    setX(qRound(x()));
    setY(qRound(y()));
    return *this;
}
