/**
 * @brief Geographic type enumerations and string-conversion helpers for Qt geometry types.
 */
#ifndef GEOTYPES_H
#define GEOTYPES_H

#include <QList>
#include <QRect>
#include <QString>

/**
 * @brief Geographic namespace providing cardinal direction enumerations and geometry string helpers.
 */
namespace Geo {

/**
 * @brief Cardinal compass directions used by geographic coordinates.
 */
enum CardinalDirection
{
    InvalidCardinalDirection = 0,   ///< Sentinel value for uninitialized direction
    North = 1,                      ///< North latitude hemisphere
    East  = 2,                      ///< East longitude hemisphere
    South = 3,                      ///< South latitude hemisphere
    West  = 4                       ///< West longitude hemisphere
};

/**
 * @brief Format a QRect as a human-readable "x,y  w,h" string.
 * @param rect Rectangle to format
 * @return String of the form "x,y W,H  w,h W,H"
 */
static inline QString string(const QRect& rect)
{
    return QString("x,y %1,%2  w,h %3,%4").arg(rect.topLeft().x()).arg(rect.topLeft().y()).arg(rect.width()).arg(rect.height());
}

/**
 * @brief Format a QSize as a human-readable "w,h" string.
 * @param size Size to format
 * @return String of the form "w,h W,H"
 */
static inline QString string(const QSize& size)
{
    return QString("w,h %1,%2").arg(size.width()).arg(size.height());
}

/**
 * @brief Format an integer QPoint as a "x,y" string.
 * @param point Point to format
 * @return String of the form "x,y"
 */
static inline QString string(const QPoint& point)
{
    return QString("%1,%2").arg(point.x()).arg(point.y());
}

/**
 * @brief Format a floating-point QPointF as a "x,y" string.
 * @param point Point to format
 * @return String of the form "x,y"
 */
static inline QString string(const QPointF& point)
{
    return QString("%1,%2").arg(point.x()).arg(point.y());
}

}

#endif // GEOTYPES_H
