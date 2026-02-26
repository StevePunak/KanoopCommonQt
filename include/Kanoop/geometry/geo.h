/**
 * @brief Geometry enumerations and utility functions for sides, directions, and spatial relationships.
 */
#ifndef GEO_H
#define GEO_H

#include <QList>
#include "Kanoop/kanoopcommon.h"

/**
 * @brief Geometry namespace providing enumerations and free functions for spatial reasoning.
 */
namespace Geo {

/**
 * @brief Bitmask identifying which side(s) of a rectangle are relevant.
 */
enum Side
{
    NoSide                  = 0x0000,   ///< No side
    Top                     = 0x0001,   ///< Top edge
    Left                    = 0x0002,   ///< Left edge
    Bottom                  = 0x0004,   ///< Bottom edge
    Right                   = 0x0008,   ///< Right edge
    TopLeftCorner           = 0x0010,   ///< Top-left corner
    TopRightCorner          = 0x0020,   ///< Top-right corner
    BottomLeftCorner        = 0x0040,   ///< Bottom-left corner
    BottomRightCorner       = 0x0080,   ///< Bottom-right corner
};
Q_DECLARE_FLAGS(Sides, Side)
Q_DECLARE_OPERATORS_FOR_FLAGS(Sides)

/**
 * @brief Cardinal directions, aliased to the corresponding Side values.
 */
enum Direction
{
    NoDirection = NoSide,    ///< No direction
    Up = Top,                ///< Upward (toward smaller Y)
    Down = Bottom,           ///< Downward (toward larger Y)
    ToLeft = Left,           ///< Leftward (toward smaller X)
    ToRight = Right          ///< Rightward (toward larger X)
};
Q_DECLARE_FLAGS(Directions, Direction)
Q_DECLARE_OPERATORS_FOR_FLAGS(Directions)

/**
 * @brief Spatial relationship between two geometric objects.
 */
enum SpatialRelationship {
    NoRelationship              = 0x0000,   ///< No defined relationship
    IntersectsWith              = 0x0001,   ///< Objects intersect
    Contains                    = 0x0002,   ///< Origin contains other
    ContainedBy                 = 0x0004,   ///< Origin is contained by other
    AwayFrom                    = 0x0008,   ///< Moving away from other
    Towards                     = 0x0010,   ///< Moving toward other
    Above                       = 0x0020,   ///< Origin is above other
    Below                       = 0x0040,   ///< Origin is below other
    ToLeftOf                    = 0x0080,   ///< Origin is to the left of other
    ToRightOf                   = 0x0100,   ///< Origin is to the right of other
    Contained                   = 0x0200,   ///< Origin is contained within other
};
Q_DECLARE_FLAGS(SpatialRelationships, SpatialRelationship)
Q_DECLARE_OPERATORS_FOR_FLAGS(SpatialRelationships)

/**
 * @brief Coordinate axes.
 */
enum Axis {
    NoAxis                      = 0x0000,   ///< No axis
    XAxis                       = 0x0001,   ///< X axis
    YAxis                       = 0x0002,   ///< Y axis
    ZAxis                       = 0x0004,   ///< Z axis
};
Q_DECLARE_FLAGS(Axes, Axis)
Q_DECLARE_OPERATORS_FOR_FLAGS(Axes)

/**
 * @brief Return a list of all four cardinal directions.
 * @return List containing Up, Down, ToLeft, ToRight
 */
KANOOP_EXPORT QList<Direction> allDirections();

/**
 * @brief Convert a Direction to the corresponding Side value.
 * @param direction Direction to convert
 * @return Equivalent Side flag
 */
KANOOP_EXPORT Side directionToSide(Direction direction);

/**
 * @brief Convert a Side to the corresponding Direction value.
 * @param side Side to convert
 * @return Equivalent Direction value
 */
KANOOP_EXPORT Direction sideToDirection(Side side);

/**
 * @brief Convert a cardinal Direction to a compass bearing in degrees.
 * @param direction Direction to convert
 * @return Bearing in degrees (0 = Up/North, 90 = Right/East, etc.)
 */
KANOOP_EXPORT double directionToBearing(Direction direction);

/**
 * @brief Convert a compass bearing to the nearest cardinal Direction.
 * @param bearing Bearing in degrees
 * @return Nearest cardinal Direction
 */
KANOOP_EXPORT Direction bearingToDirection(double bearing);

/**
 * @brief Return the opposite of a cardinal Direction.
 * @param direction Input direction
 * @return Opposite direction (e.g. Up -> Down)
 */
KANOOP_EXPORT Direction oppositeDirection(Direction direction);

} // namespace Geo

#endif // GEO_H
