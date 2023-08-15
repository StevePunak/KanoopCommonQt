#ifndef GEO_H
#define GEO_H

#include <QList>

namespace Geo {

enum Side
{
    NoSide                  = 0x0000,
    Top                     = 0x0001,
    Left                    = 0x0002,
    Bottom                  = 0x0004,
    Right                   = 0x0008,
    TopLeftCorner           = 0x0010,
    TopRightCorner          = 0x0020,
    BottomLeftCorner        = 0x0040,
    BottomRightCorner       = 0x0080,
};

enum Direction
{
    NoDirection = NoSide,
    Up = Top,
    Down = Bottom,
    ToLeft = Left,
    ToRight = Right
};

enum SpatialRelationship {
    NoRelationship              = 0x0000,
    IntersectsWith              = 0x0001,
    Contains                    = 0x0002,
    ContainedBy                 = 0x0004,
    AwayFrom                    = 0x0008,
    Towards                     = 0x0010,
    Above                       = 0x0020,
    Below                       = 0x0040,
    ToLeftOf                    = 0x0080,
    ToRightOf                   = 0x0100,
    Contained                   = 0x0200,
};

QList<Direction> allDirections();
Side directionToSide(Direction direction);
Direction sideToDirection(Side side);
double directionToBearing(Direction direction);
Direction bearingToDirection(double bearing);
Direction oppositeDirection(Direction direction);

} // namespace Geo

#endif // GEO_H
