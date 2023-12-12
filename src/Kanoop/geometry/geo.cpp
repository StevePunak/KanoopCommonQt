#include "Kanoop/geometry/geo.h"

using namespace Geo;

QList<Geo::Direction> Geo::allDirections()
{
    static const QList<Direction> directions =
        {
            Up, Down, ToLeft, ToRight
        };
    return directions;
}

Geo::Side Geo::directionToSide(Direction direction)
{
    return (Side)direction;
}

Geo::Direction Geo::sideToDirection(Side side)
{
    return (Direction)side;
}

double Geo::directionToBearing(Direction direction)
{
    double result = 0;
    switch(direction) {
    case Up:
        result = 0;
        break;
    case Down:
        result = 180;
        break;
    case ToLeft:
        result = 270;
        break;
    case ToRight:
        result = 90;
        break;
    default:
        break;
    }
    return result;
}

Direction Geo::oppositeDirection(Direction direction)
{
    Direction result = NoDirection;
    switch(direction) {
    case Up:
        result = Down;
        break;
    case Down:
        result = Up;
        break;
    case ToLeft:
        result = ToRight;
        break;
    case ToRight:
        result = ToLeft;
        break;
    default:
        break;
    }
    return result;
}

Direction Geo::bearingToDirection(double bearing)
{
    Direction result = NoDirection;
    if(bearing == 0) {
        result = Up;
    }
    else if(bearing == 180) {
        result = Down;
    }
    else if(bearing == 90) {
        result = ToRight;
    }
    else if(bearing == 270) {
        result = ToLeft;
    }
    return result;
}
