#include "point.h"

using namespace Geo;

Point &Point::move(Direction direction, double amount)
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
    return *this;
}

Point &Point::round()
{
    setX(qRound(x()));
    setY(qRound(y()));
    return *this;
}

Point Point::List::topLeft() const
{
    Point result;
    for(int i = 0;i < count();i++) {
        Point p = this->at(i);
        if(i == 0 || (p.x() <= result.x() && p.y() <= result.y())) {
            result = p;
        }
    }
    return result;
}

Point Point::List::topRight() const
{
    Point result;
    for(int i = 0;i < count();i++) {
        Point p = this->at(i);
        if(i == 0 || (p.x() >= result.x() && p.y() <= result.y())) {
            result = p;
        }
    }
    return result;
}

Point Point::List::bottomLeft() const
{
    Point result;
    for(int i = 0;i < count();i++) {
        Point p = this->at(i);
        if(i == 0 || (p.x() <= result.x() && p.y() >= result.y())) {
            result = p;
        }
    }
    return result;
}

Point Point::List::bottomRight() const
{
    Point result;
    for(int i = 0;i < count();i++) {
        Point p = this->at(i);
        if(i == 0 || (p.x() >= result.x() && p.y() >= result.y())) {
            result = p;
        }
    }
    return result;
}
