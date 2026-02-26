/**
 * @brief A 2D rectangle extending QRectF with edge, corner, and geometric query helpers.
 */
#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <QMap>
#include <QRectF>
#include "point.h"
#include "line.h"
#include "Kanoop/kanoopcommon.h"

/**
 * @brief Extends QRectF with edge and corner accessors, containment tests, and scale/grow/shrink operations.
 */
class KANOOP_EXPORT Rectangle : public QRectF
{
public:
    /** @brief Default constructor — creates a null rectangle. */
    Rectangle() : QRectF() {}
    /** @brief Construct from a QRectF. */
    Rectangle(const QRectF& other) : QRectF(other) {}
    /** @brief Construct from a QRect (integer coordinates). */
    Rectangle(const QRect& other) : QRectF(other) {}
    /**
     * @brief Construct from a top-left point and size.
     * @param topLeft Top-left corner
     * @param size Dimensions of the rectangle
     */
    Rectangle(const QPointF& topLeft, const QSizeF& size) : QRectF(topLeft, size) {}
    /**
     * @brief Construct from explicit position and dimensions.
     * @param x Left edge X coordinate
     * @param y Top edge Y coordinate
     * @param w Width
     * @param h Height
     */
    Rectangle(double x, double y, int w, int h) : QRectF(x, y, w, h) {}

    /**
     * @brief Build the bounding rectangle of a list of points.
     * @param points List of points to enclose
     * @return Bounding Rectangle
     */
    static Rectangle fromPoints(const Point::List& points);

    /**
     * @brief Build the bounding rectangle of two points.
     * @param p1 First corner point
     * @param p2 Opposite corner point
     * @return Rectangle with p1 and p2 as opposite corners
     */
    static Rectangle fromPoints(const Point& p1, const Point& p2);

    /**
     * @brief Create a rectangle centred on a line with a given width.
     * @param centerLine Centre line of the rectangle
     * @param expand Half-width of the rectangle (perpendicular to the line)
     * @return Rectangle enclosing the line
     */
    static Rectangle fromCenterLine(const Line& centerLine, double expand);

    /**
     * @brief Create a rectangle centred on a point with a given half-size.
     * @param centerPoint Centre of the rectangle
     * @param expand Half-dimension in each direction
     * @return Square rectangle centred on centerPoint
     */
    static Rectangle fromCenterPoint(const Point& centerPoint, double expand);

    /**
     * @brief Parse a Rectangle from a string produced by toString().
     * @param value String representation
     * @return Parsed Rectangle
     */
    static Rectangle fromString(const QString& value);

    /**
     * @brief Return the corner of this rectangle closest to an origin point.
     * @param origin Reference point
     * @return Closest corner Point
     */
    Point closestCorner(const Point& origin) const;

    /**
     * @brief Return all four corners of the rectangle.
     * @return List of the four corner Points (top-left, top-right, bottom-right, bottom-left)
     */
    Point::List corners() const;

    /**
     * @brief Return all four edges of the rectangle as lines.
     * @return Line list of the four edges
     */
    Line::List edges() const;

    /**
     * @brief Return a map from Side to edge Line for all four edges.
     * @return Map keyed by Geo::Side containing each edge
     */
    QMap<Geo::Side, Line> edgeMap() const;

    /** @brief Return the two vertical edges. */
    Line::List verticalLines() const;
    /** @brief Return the two horizontal edges. */
    Line::List horizontalLines() const;

    /** @brief Return the top edge as a Line. */
    Line topEdge() const;
    /** @brief Return the bottom edge as a Line. */
    Line bottomEdge() const;
    /** @brief Return the left edge as a Line. */
    Line leftEdge() const;
    /** @brief Return the right edge as a Line. */
    Line rightEdge() const;

    /**
     * @brief Return the edge closest to a given point.
     * @param point Reference point
     * @return Closest edge Line
     */
    Line closestEdge(const Point& point);

    /**
     * @brief Test whether a point lies on any edge, outputting the edge.
     * @param point Point to test
     * @param foundEdge Output set to the edge containing the point
     * @return true if the point is on an edge
     */
    bool isPointOnEdge(const Point& point, Line &foundEdge) const;

    /**
     * @brief Test whether a point lies on any edge.
     * @param point Point to test
     * @return true if the point is on an edge
     */
    bool isPointOnEdge(const Point& point) const;

    /**
     * @brief Test whether a line's endpoints include any point within this rectangle.
     * @param line Line to test
     * @return true if either endpoint is inside the rectangle
     */
    bool containsAnyPoint(const Line& line) const;

    /** @brief Return the geometric centre point of the rectangle. */
    Point centerPoint() const;

    /**
     * @brief Return which side of the rectangle is closest to a point.
     * @param point Reference point
     * @return Closest Geo::Side value
     */
    Geo::Side closestSideToPoint(const Point &point) const;

    /**
     * @brief Scale the rectangle dimensions by a factor, keeping the top-left corner fixed.
     * @param scale Scale factor
     * @return Reference to this rectangle
     */
    Rectangle& scale(double scale);

    /**
     * @brief Return a scaled copy of this rectangle.
     * @param scale Scale factor
     * @return New scaled Rectangle
     */
    Rectangle scaled(double scale) const;

    /**
     * @brief Grow the rectangle by expanding all edges outward.
     * @param amount Amount to expand each edge
     * @return Reference to this rectangle
     */
    Rectangle& grow(double amount);

    /**
     * @brief Return a grown copy of this rectangle.
     * @param amount Amount to expand each edge
     * @return New grown Rectangle
     */
    Rectangle grown(double amount) const;

    /**
     * @brief Shrink the rectangle by contracting all edges inward.
     * @param amount Amount to contract each edge
     * @return Reference to this rectangle
     */
    Rectangle& shrink(double amount) { return grow(-amount); }

    /**
     * @brief Return a shrunk copy of this rectangle.
     * @param amount Amount to contract each edge
     * @return New shrunk Rectangle
     */
    Rectangle shrunk(double amount) const { return grown(-amount); }

    /** @brief Format this rectangle as a human-readable string. */
    QString toString() const;

    /** @brief A simple list type for Rectangle objects. */
    class List : public QList<Rectangle> {};

};

#endif // RECTANGLE_H
