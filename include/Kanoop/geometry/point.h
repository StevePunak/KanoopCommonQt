/**
 * @brief A 2D floating-point point extending QPointF with movement and spatial query methods.
 */
#ifndef POINT_H
#define POINT_H
#include <QList>
#include <QPointF>
#include "Kanoop/kanoopcommon.h"

#include "geo.h"

/**
 * @brief Extends QPointF with movement transforms, string conversion, and a list subclass.
 */
class KANOOP_EXPORT Point : public QPointF
{
public:
    /** @brief Default constructor — creates a point at the origin. */
    Point() : QPointF() {}
    /** @brief Construct from a QPointF. */
    Point(const QPointF& other) : QPointF(other) {}
    /** @brief Construct from a QPoint (integer coordinates). */
    Point(const QPoint& other) : QPointF(other) {}
    /**
     * @brief Construct from explicit X and Y coordinates.
     * @param x X coordinate
     * @param y Y coordinate
     */
    Point(double x, double y) : QPointF(x, y) {}

    /** @brief Test whether this point is to the left of other (smaller X). */
    bool isLeftOf(const QPointF& other) const { return x() < other.x(); }
    /** @brief Test whether this point is to the right of other (larger X). */
    bool isRightOf(const QPointF& other) const { return x() > other.x(); }
    /** @brief Test whether this point is above other (smaller Y). */
    bool isAbove(const QPointF& other) const { return y() < other.y(); }
    /** @brief Test whether this point is below other (larger Y). */
    bool isBelow(const QPointF& other) const { return y() > other.y(); }

    /**
     * @brief Move this point in a cardinal direction by the given amount.
     * @param direction Cardinal direction to move
     * @param amount Distance to move
     * @return Reference to this point
     */
    Point& move(Geo::Direction direction, double amount);

    /**
     * @brief Move this point by a bearing and distance.
     * @param bearing Compass bearing in degrees
     * @param distance Distance to move
     * @return Reference to this point
     */
    Point& move(double bearing, double distance);

    /**
     * @brief Translate this point by a (dx, dy) delta.
     * @param dx Horizontal displacement
     * @param dy Vertical displacement
     * @return Reference to this point
     */
    Point& moveDelta(double dx, double dy);

    /** @brief Round both coordinates to the nearest integer in place. */
    Point& round();

    /**
     * @brief Offset this point by the given (x, y) amounts.
     * @param x Horizontal offset
     * @param y Vertical offset
     * @return Reference to this point
     */
    Point& offset(double x, double y);

    /**
     * @brief Compute the (dx, dy) vector from this point to another.
     * @param other Target point
     * @return Delta point representing the displacement
     */
    Point delta(const Point& other) const;

    /**
     * @brief Format this point as "x, y".
     * @return String representation
     */
    QString toString() const { return QString("%1, %2").arg(x()).arg(y()); }

    /**
     * @brief Parse a Point from a string produced by toString().
     * @param value String of the form "x, y"
     * @return Parsed Point
     */
    static Point fromString(const QString& value);

    /**
     * @brief A list of Point objects with corner-finding helpers.
     */
    class List : public QList<Point>
    {
    public:
        /** @brief Default constructor. */
        List() {}

        /** @brief Construct from a QList<Point>. */
        List(const QList<Point>& other)
        {
            for(const Point& p : other) {
                append(p);
            }
        }

        /** @brief Return the top-left point (minimum X and Y). */
        Point topLeft() const;
        /** @brief Return the top-right point (maximum X, minimum Y). */
        Point topRight() const;
        /** @brief Return the bottom-left point (minimum X, maximum Y). */
        Point bottomLeft() const;
        /** @brief Return the bottom-right point (maximum X and Y). */
        Point bottomRight() const;

        /** @brief Convert to a list of integer QPoint objects. */
        QList<QPoint> toPointList() const
        {
            QList<QPoint> result;
            for(const Point& point : *this) {
                result.append(point.toPoint());
            }
            return result;
        }
    };
};

#endif // POINT_H
