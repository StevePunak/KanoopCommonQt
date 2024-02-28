#ifndef GEO_H
#define GEO_H

#include <QList>
#include "Kanoop/kanoopcommon.h"

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
Q_DECLARE_FLAGS(Sides, Side)
Q_DECLARE_OPERATORS_FOR_FLAGS(Sides)

enum Direction
{
    NoDirection = NoSide,
    Up = Top,
    Down = Bottom,
    ToLeft = Left,
    ToRight = Right
};
Q_DECLARE_FLAGS(Directions, Direction)
Q_DECLARE_OPERATORS_FOR_FLAGS(Directions)

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
Q_DECLARE_FLAGS(SpatialRelationships, SpatialRelationship)
Q_DECLARE_OPERATORS_FOR_FLAGS(SpatialRelationships)

enum Axis {
    NoAxis                      = 0x0000,
    XAxis                       = 0x0001,
    YAxis                       = 0x0002,
    ZAxis                       = 0x0004,
};
Q_DECLARE_FLAGS(Axes, Axis)
Q_DECLARE_OPERATORS_FOR_FLAGS(Axes)

KANOOP_EXPORT QList<Direction> allDirections();
KANOOP_EXPORT Side directionToSide(Direction direction);
KANOOP_EXPORT Direction sideToDirection(Side side);
KANOOP_EXPORT double directionToBearing(Direction direction);
KANOOP_EXPORT Direction bearingToDirection(double bearing);
KANOOP_EXPORT Direction oppositeDirection(Direction direction);

} // namespace Geo

#endif // GEO_H
