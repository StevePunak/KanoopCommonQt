/**
 * @brief Static 2D planar geometry calculation helpers.
 */
#ifndef FLATGEO_H
#define FLATGEO_H

#include "point.h"
#include "Kanoop/kanoopcommon.h"
#include <QRect>

class Circle;
class Line;
class Angle;
class QRect;

/**
 * @brief Static helper methods for 2D (flat / Cartesian) geometric computations.
 *
 * All coordinates use Qt's screen coordinate system where Y increases downward.
 * Angles are in degrees unless otherwise specified.
 */
class KANOOP_EXPORT FlatGeo
{
public:
    /**
     * @brief Compute the signed vector angle between two lines.
     * @param l1 First line
     * @param l2 Second line
     * @return Signed angle in degrees
     */
    static double vectorAngle(const Line& l1, const Line& l2);

    /**
     * @brief Compute the angle between two lines as an Angle object.
     * @param l1 First line
     * @param l2 Second line
     * @return Angle between the lines
     */
    static Angle angle(const Line& l1, const Line& l2);

    /**
     * @brief Compute the angle at a vertex formed by two rays.
     * @param p1 First point on the first ray
     * @param vertex Vertex of the angle
     * @param p2 First point on the second ray
     * @return Angle at the vertex
     */
    static Angle angle(const QPointF& p1, const QPointF& vertex, const QPointF& p2);

    /**
     * @brief Compute the Euclidean distance between two points.
     * @param p1 First point
     * @param p2 Second point
     * @return Distance in scene units
     */
    static double distance(const QPointF& p1, const QPointF& p2);

    /**
     * @brief Convert radians to degrees.
     * @param angle Angle in radians
     * @return Equivalent angle in degrees
     */
    static double degrees(double angle);

    /**
     * @brief Convert degrees to radians.
     * @param angle Angle in degrees
     * @return Equivalent angle in radians
     */
    static double radians(double angle);

    /**
     * @brief Compute the absolute angular difference between two lines.
     * @param l1 First line
     * @param l2 Second line
     * @return Absolute angular difference in degrees
     */
    static double angularDifference(const Line& l1, const Line& l2);

    /**
     * @brief Compute a point at a given bearing and distance from a source point.
     * @param from Origin point
     * @param bearing Compass bearing in degrees (0 = up/north)
     * @param distance Distance from origin
     * @return Resulting Point
     */
    static Point getPoint(const QPointF& from, double bearing, double distance);

    /**
     * @brief Compute the intersection point of two lines.
     * @param l1 First line
     * @param l2 Second line
     * @return Intersection point, or an undefined point if lines are parallel
     */
    static QPointF intersection(const Line& l1, const Line& l2);

    /**
     * @brief Test whether a point lies within a circle.
     * @param point Point to test
     * @param circle Circle to test against
     * @return true if the point is inside or on the circle boundary
     */
    static bool isInCircle(const QPointF& point, const Circle& circle);

    /**
     * @brief Compute the A, B, C coefficients of the line equation Ax + By + C = 0.
     * @param line Input line
     * @param a Output coefficient A
     * @param b Output coefficient B
     * @param c Output coefficient C
     */
    static void getLineABC(const Line& line, double& a, double& b, double& c);

    /**
     * @brief Move a point by a given bearing and distance.
     * @param point Starting point
     * @param bearing Direction of movement in degrees
     * @param distance Distance to move
     * @return New QPointF after movement
     */
    static QPointF move(const QPointF& point, double bearing, double distance);

    /**
     * @brief Rotate a point around a centroid by an angle.
     * @param point Point to rotate
     * @param centroid Centre of rotation
     * @param angle Rotation angle in degrees (clockwise)
     * @return Rotated QPointF
     */
    static QPointF rotate(const QPointF& point, const QPointF& centroid, double angle);

    /**
     * @brief Test whether two points are equal within a given precision.
     * @param p1 First point
     * @param p2 Second point
     * @param precision Number of decimal places to compare (0 = integer precision)
     * @return true if the points are equal within the specified precision
     */
    static bool arePointsEqual(const QPointF& p1, const QPointF& p2, int precision = 0);

    /** @brief Test whether point is to the left of other (smaller X). */
    static inline bool isPointLeftOf(const QPointF& point, const QPointF& other)
    {
        return point.x() < other.x();
    }
    /** @brief Test whether point is to the right of other (larger X). */
    static inline bool isPointRightOf(const QPointF& point, const QPointF& other)
    {
        return point.x() > other.x();
    }
    /** @brief Test whether point is above other (smaller Y). */
    static inline bool isPointAbove(const QPointF& point, const QPointF& other)
    {
        return point.y() < other.y();
    }
    /** @brief Test whether point is below other (larger Y). */
    static inline bool isPointBelow(const QPointF& point, const QPointF& other)
    {
        return point.y() > other.y();
    }
    /** @brief Test whether rect's right edge is left of other. */
    static inline bool isRectLeftOf(const QRectF& rect, const QPointF& other)
    {
        return rect.x() + rect.width() < other.x();
    }
    /** @brief Test whether rect's left edge is right of other. */
    static inline bool isRectRightOf(const QRectF& rect, const QPointF& other)
    {
        return rect.x() > other.x();
    }
    /** @brief Test whether rect's bottom edge is above other. */
    static inline bool isRectAbove(const QRectF& rect, const QPointF& other)
    {
        return rect.y() + rect.height() < other.y();
    }
    /** @brief Test whether rect's top edge is below other. */
    static inline bool isRectBelow(const QRectF& rect, const QPointF& other)
    {
        return rect.y() > other.y();
    }
    /** @brief Test whether other is to the left of rect. */
    static inline bool isRectLeftOf(const QRectF& rect, const QRectF& other)
    {
        return other.x() < rect.x();
    }
    /** @brief Test whether other is to the right of rect. */
    static inline bool isRectRightOf(const QRectF& rect, const QRectF& other)
    {
        return other.x() > rect.x() + rect.width();
    }
    /** @brief Test whether other is above rect. */
    static inline bool isRectAbove(const QRectF& rect, const QRectF& other)
    {
        return other.y() < rect.y();
    }
    /** @brief Test whether other is below rect. */
    static inline bool isRectBelow(const QRectF& rect, const QRectF& other)
    {
        return other.y() > rect.y() + rect.height();
    }

    /**
     * @brief Determine the spatial relationship of a point relative to an origin point.
     * @param origin Reference point
     * @param other Point to compare
     * @return SpatialRelationship flags describing the relative position
     */
    static Geo::SpatialRelationship relationTo(const QPointF& origin, const QPointF& other);

    /**
     * @brief Determine the spatial relationship of a point relative to a rectangle.
     * @param origin Reference rectangle
     * @param other Point to compare
     * @return SpatialRelationship flags describing the relative position
     */
    static Geo::SpatialRelationship relationTo(const QRectF& origin, const QPointF& other);

    /**
     * @brief Determine the spatial relationship of a rectangle relative to an origin point.
     * @param origin Reference point
     * @param other Rectangle to compare
     * @return SpatialRelationship flags describing the relative position
     */
    static Geo::SpatialRelationship relationTo(const QPointF& origin, const QRectF& other);

    /**
     * @brief Determine the spatial relationship between two rectangles.
     * @param origin Reference rectangle
     * @param other Rectangle to compare
     * @return SpatialRelationship flags describing the relative position
     */
    static Geo::SpatialRelationship relationTo(const QRectF& origin, const QRectF& other);

    /**
     * @brief Format an integer point as a comma-separated string.
     * @param p Point to format
     * @return String of the form "x, y"
     */
    static QString makePointString(const QPoint& p);

    /**
     * @brief Format a floating-point point as a comma-separated string.
     * @param p Point to format
     * @return String of the form "x, y"
     */
    static QString makePointString(const QPointF& p);

    /** @brief Number of decimal places used for rounding in geometric comparisons. */
    static const int Precision = 6;

private:
    /** @brief Alternative angle computation implementation. */
    static Angle anglev2(const Line& l1, const Line& l2);
    /** @brief Format a double rounded to the given precision as a string. */
    static QString roundedString(double value, int precision);
    /** @brief Round a double to the given number of decimal places. */
    static double roundedDouble(double value, int precision);
};


#endif // FLATGEO_H
