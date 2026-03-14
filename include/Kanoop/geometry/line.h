/**
 * @brief A 2D line segment defined by two endpoints, with extensive geometric query support.
 */
#ifndef LINE_H
#define LINE_H

#include <QLineF>
#include <QList>
#include <QPoint>
#include <QRect>

#include "Kanoop/kanoopcommon.h"
#include "flatgeo.h"
#include "point.h"

class QLineF;
class QLine;
class Circle;
class Rectangle;

/**
 * @brief Represents a 2D line segment between two Point endpoints.
 *
 * Provides length, slope, bearing, midpoint, intersection, containment, and
 * spatial relationship queries as well as movement and rotation transforms.
 */
class KANOOP_EXPORT Line
{
public:
    /** @brief Default constructor — creates a zero-length line at the origin. */
    Line() {}

    /**
     * @brief Construct a line from two integer points.
     * @param p1 Start point
     * @param p2 End point
     */
    Line(const QPoint& p1, const QPoint& p2) :
        _p1(p1), _p2(p2) {}

    /**
     * @brief Construct a line from two floating-point points.
     * @param p1 Start point
     * @param p2 End point
     */
    Line(const QPointF& p1, const QPointF& p2) :
        _p1(p1), _p2(p2) {}

    /**
     * @brief Construct a line from an origin point, bearing, and distance.
     * @param origin Start point
     * @param bearing Compass bearing in degrees
     * @param distance Length of the line
     */
    Line(const QPointF& origin, double bearing, double distance);

    /**
     * @brief Construct a line from an origin point, cardinal direction, and distance.
     * @param origin Start point
     * @param direction Cardinal direction of travel
     * @param distance Length of the line
     */
    Line(const QPointF& origin, Geo::Direction direction, double distance);

    /**
     * @brief Construct from a QLineF.
     * @param other Source QLineF to copy endpoints from
     */
    Line(const QLineF& other) :
        Line(other.p1(), other.p2()) {}

    /**
     * @brief Construct from a QLine.
     * @param other Source QLine to copy endpoints from
     */
    Line(const QLine& other) :
        Line(other.p1(), other.p2()) {}

    /**
     * @brief Equality comparison — endpoints must match exactly.
     * @param other Line to compare against
     * @return true if both endpoints match
     */
    bool operator ==(const Line& other) const { return _p1 == other._p1 && _p2 == other._p2; }
    /**
     * @brief Inequality comparison.
     * @param other Line to compare against
     * @return true if the lines differ
     */
    bool operator !=(const Line& other) const { return (*this == other) == false; }

    /**
     * @brief Parse a Line from a string produced by toString().
     * @param value String representation
     * @return Parsed Line
     */
    static Line fromString(const QString& value);

    /**
     * @brief Return the start point.
     * @return Start Point
     */
    Point p1() const { return _p1; }

    /**
     * @brief Set the start point.
     * @param value New start point
     */
    void setP1(const Point& value) { _p1 = value; }

    /**
     * @brief Return the end point.
     * @return End Point
     */
    Point p2() const { return _p2; }

    /**
     * @brief Set the end point.
     * @param value New end point
     */
    void setP2(const Point& value) { _p2 = value; }

    /**
     * @brief A list of Line objects with spatial query helpers.
     */
    class KANOOP_EXPORT List : public QList<Line>
    {
    public:
        /** @brief Default constructor. */
        List() : QList<Line>() {}

        /**
         * @brief Construct from a QList<Line>.
         * @param other Source list to copy lines from
         */
        List(const QList<Line>& other) {
            for(const Line& line : other) {
                append(line);
            }
        }

        /**
         * @brief Build a list of lines connecting adjacent points in a QList<QPoint>.
         * @param points List of integer points
         * @return List of lines connecting consecutive points
         */
        static List fromPoints(const QList<QPoint>& points);

        /**
         * @brief Build a list of lines connecting adjacent points in a QList<QPointF>.
         * @param points List of floating-point points
         * @return List of lines connecting consecutive points
         */
        static List fromPoints(const QList<QPointF>& points);

        /**
         * @brief Parse a List from a string produced by toString().
         * @param value String representation
         * @return Parsed List
         */
        static List fromString(const QString& value);

        /** @brief Return the longest horizontal line in the list.
         * @return Longest horizontal Line */
        Line longestHorizontalLine();
        /** @brief Return the longest vertical line in the list.
         * @return Longest vertical Line */
        Line longestVerticalLine();

        /**
         * @brief Test whether the list contains a line with the same two endpoints.
         * @param line Line to search for
         * @return true if a matching line is found
         */
        bool containsLineWithSameEndpoints(const Line& line) const;

        /** @brief Return the line with the smallest Y start or end coordinate.
         * @return Highest Line */
        Line highest() const;
        /** @brief Return the line with the largest Y start or end coordinate.
         * @return Lowest Line */
        Line lowest() const;
        /** @brief Return the shortest line in the list.
         * @return Shortest Line */
        Line shortest() const;
        /** @brief Return the longest line in the list.
         * @return Longest Line */
        Line longest() const;

        /**
         * @brief Return the point on any line closest to the given point.
         * @param other Reference point
         * @return Closest point on any line
         */
        QPointF closestPointTo(const QPointF& other);

        /**
         * @brief Return the closest point, and output which line and distance produced it.
         * @param other Reference point
         * @param closestLine Output set to the line containing the closest point
         * @param closestDistance Output set to the distance to the closest point
         * @return Closest point on any line
         */
        QPointF closestPointTo(const QPointF& other, Line& closestLine, double &closestDistance);

        /** @brief Return the minimum X coordinate across all line endpoints.
         * @return Minimum X value */
        double minX() const;
        /** @brief Return the maximum X coordinate across all line endpoints.
         * @return Maximum X value */
        double maxX() const;
        /** @brief Return the minimum Y coordinate across all line endpoints.
         * @return Minimum Y value */
        double minY() const;
        /** @brief Return the maximum Y coordinate across all line endpoints.
         * @return Maximum Y value */
        double maxY() const;

        /** @brief Return the bounding rectangle of all lines in the list.
         * @return Bounding Rectangle */
        Rectangle boundingRectangle() const;

        /**
         * @brief Return the first line that contains the given point.
         * @param point Point to search for
         * @return Matching line, or an invalid line if not found
         */
        Line lineContainingPoint(const Point& point) const;

        /**
         * @brief Return the first line near the given point within a margin.
         * @param point Reference point
         * @param margin Maximum allowed distance
         * @return Nearby line, or an invalid line if not found
         */
        Line lineNearPoint(const Point& point, int margin) const;

        /** @brief Return the total combined length of all lines.
         * @return Sum of all line lengths */
        double totalLength() const;

        /**
         * @brief Move all lines by the given delta.
         * @param dx Horizontal displacement
         * @param dy Vertical displacement
         * @return Reference to this list
         */
        List &moveDelta(double dx, double dy);

        /** @brief Convert all lines to a list of QLineF objects.
         * @return QList of QLineF */
        QList<QLineF> toQLineFList() const;
        /** @brief Convert all lines to a list of QLine objects.
         * @return QList of QLine */
        QList<QLine> toQLineList() const;

        /** @brief Format the list as a human-readable string.
         * @return String representation */
        QString toString() const;
    };

    /** @brief Return the midpoint of this line.
     * @return Midpoint */
    Point midpoint() const;
    /** @brief Return the length (Euclidean distance between endpoints).
     * @return Length of the line */
    double length() const;
    /** @brief Return the slope (rise/run), or infinity for vertical lines.
     * @return Slope value */
    double slope() const;
    /** @brief Return the Y-intercept of the line's equation.
     * @return Y-intercept */
    double intercept() const;
    /** @brief Test whether this line is exactly vertical.
     * @return true if vertical */
    bool isVertical() const;
    /** @brief Test whether this line is exactly horizontal.
     * @return true if horizontal */
    bool isHorizontal() const;
    /** @brief Return the compass bearing from p1 to p2, in degrees.
     * @return Bearing in degrees */
    double bearing() const;
    /** @brief Return the angle of this line as an Angle object.
     * @return Angle of the line */
    Angle angle() const;

    /**
     * @brief Return the perpendicular distance from this line to a point.
     * @param to Point to measure distance from
     * @return Perpendicular distance
     */
    double distanceTo(const QPointF& to) const;

    /**
     * @brief Return the point on this line closest to the given point.
     * @param point Reference point
     * @return Closest Point on this line segment
     */
    Point closestPointTo(const QPointF& point) const;

    /**
     * @brief Return the closest point and output the distance.
     * @param point Reference point
     * @param distance Output set to the distance
     * @return Closest Point on this line segment
     */
    Point closestPointTo(const QPointF& point, double& distance) const;

    /**
     * @brief Return the endpoint furthest from the given point.
     * @param point Reference point
     * @return The furthest of p1 and p2
     */
    Point furthestPointFrom(const QPointF& point) const;

    /** @brief Return the endpoint with the smallest Y coordinate.
     * @return Top-most Point */
    Point topMostPoint() const;
    /** @brief Return the endpoint with the largest Y coordinate.
     * @return Bottom-most Point */
    Point bottomMostPoint() const;
    /** @brief Return the endpoint with the smallest X coordinate.
     * @return Left-most Point */
    Point leftMostPoint() const;
    /** @brief Return the endpoint with the largest X coordinate.
     * @return Right-most Point */
    Point rightMostPoint() const;

    /**
     * @brief Test whether the line spans an X range.
     * @param x1 First X boundary
     * @param x2 Second X boundary
     * @return true if any part of the line falls between x1 and x2
     */
    bool hasXBetween(double x1, double x2) const;

    /**
     * @brief Test whether the line crosses a specific X coordinate.
     * @param x X coordinate to test
     * @return true if the line crosses x
     */
    bool crossesX(double x) const;

    /**
     * @brief Test whether the line spans a Y range.
     * @param y1 First Y boundary
     * @param y2 Second Y boundary
     * @return true if any part of the line falls between y1 and y2
     */
    bool hasYBetween(double y1, double y2) const;

    /**
     * @brief Test whether the line crosses a specific Y coordinate.
     * @param y Y coordinate to test
     * @return true if the line crosses y
     */
    bool crossesY(double y) const;

    /**
     * @brief Test whether this line intersects another.
     * @param other Line to test against
     * @return true if the segments intersect
     */
    bool intersects(const Line& other) const;

    /**
     * @brief Test whether this line intersects another and output the intersection point.
     * @param other Line to test against
     * @param intersection Output set to the intersection point
     * @return true if the segments intersect
     */
    bool intersects(const Line& other, Point &intersection) const;

    /**
     * @brief Test whether this line intersects a circle.
     * @param other Circle to test against
     * @return true if the line intersects or is tangent to the circle
     */
    bool intersects(const Circle& other) const;

    /**
     * @brief Test whether this line intersects a rectangle.
     * @param other Rectangle to test against
     * @return true if the line intersects the rectangle boundary
     */
    bool intersects(const QRectF &other) const;

    /**
     * @brief Compute the intersection point with another line.
     * @param other Line to intersect with
     * @return Intersection point (may be invalid if lines are parallel)
     */
    Point intersection(const Line& other) const;

    /**
     * @brief Test whether this line is entirely to the left of other.
     * @param other Line to compare against
     * @return true if this line is entirely to the left
     */
    bool isLeftOf(const Line& other) const;
    /**
     * @brief Test whether this line is entirely to the right of other.
     * @param other Line to compare against
     * @return true if this line is entirely to the right
     */
    bool isRightOf(const Line& other) const;
    /**
     * @brief Test whether this line is entirely above other.
     * @param other Line to compare against
     * @return true if this line is entirely above
     */
    bool isAbove(const Line& other) const;
    /**
     * @brief Test whether this line is entirely below other.
     * @param other Line to compare against
     * @return true if this line is entirely below
     */
    bool isBelow(const Line& other) const;
    /** @brief Test whether this line is perpendicular (horizontal or vertical).
     * @return true if the line is horizontal or vertical */
    bool isPerpendicular() const;
    /**
     * @brief Test whether this line shares an axis with other (both horizontal or both vertical).
     * @param other Line to compare against
     * @return true if both lines share the same axis
     */
    bool sharesAxisWith(const Line& other) const;

    /**
     * @brief Test whether this line shares an endpoint with other within a tolerance.
     * @param other Line to compare
     * @param maxDistance Maximum endpoint distance to consider shared (default 0 = exact)
     * @return true if any endpoint pair is within maxDistance of each other
     */
    bool sharesEndpointWith(const Line& other, double maxDistance = 0) const;

    /**
     * @brief Test whether this line has exactly the same two endpoints as other.
     * @param other Line to compare against
     * @return true if both endpoints match in either order
     */
    bool sharesSameEndpoints(const Line& other) const;

    /**
     * @brief Test whether a point is an endpoint of this line.
     * @param point Point to test
     * @param precision Decimal places for comparison (0 = integer)
     * @return true if point matches p1 or p2 within precision
     */
    bool isEndpoint(const QPointF& point, int precision = 0) const;

    /**
     * @brief Test whether a point lies on this line segment.
     * @param point Point to test
     * @return true if point lies on the segment
     */
    bool containsPoint(const QPointF& point) const;

    /**
     * @brief Shorten this line by removing length from the far end.
     * @param howMuch Amount to remove
     * @return Reference to this line
     */
    Line& shorten(double howMuch);

    /**
     * @brief Extend this line by adding length at the far end.
     * @param howMuch Amount to add
     * @return Reference to this line
     */
    Line& extend(double howMuch);

    /**
     * @brief Grow this line symmetrically by adding length at both ends.
     * @param howMuch Amount to add at each end
     * @return Reference to this line
     */
    Line& grow(double howMuch);

    /** @brief Return the cardinal direction of this line.
     * @return Cardinal direction */
    Geo::Direction direction() const;

    /** @brief Return the minimum X coordinate of the two endpoints.
     * @return Minimum X value */
    double minX() const;
    /** @brief Return the maximum X coordinate of the two endpoints.
     * @return Maximum X value */
    double maxX() const;
    /** @brief Return the minimum Y coordinate of the two endpoints.
     * @return Minimum Y value */
    double minY() const;
    /** @brief Return the maximum Y coordinate of the two endpoints.
     * @return Maximum Y value */
    double maxY() const;

    /** @brief Return a list containing both endpoints.
     * @return Point::List with p1 and p2 */
    Point::List points() const;

    /** @brief Round both endpoints to integer coordinates in place.
     * @return Reference to this line */
    Line& round();

    /**
     * @brief Create a rectangle of the given width centred on this line.
     * @param expandedWidth Width of the resulting rectangle
     * @return Rectangle enclosing the line
     */
    Rectangle makeRectangle(int expandedWidth) const;

    /**
     * @brief Return vertical lines along the left and right edges of a rectangle.
     * @param rect Rectangle to extract edges from
     * @return List of vertical edge lines
     */
    static List verticalLines(const QRectF& rect);

    /**
     * @brief Return horizontal lines along the top and bottom edges of a rectangle.
     * @param rect Rectangle to extract edges from
     * @return List of horizontal edge lines
     */
    static List horizontalLines(const QRectF& rect);

    /**
     * @brief Move this line by bearing and distance.
     * @param bearing Direction of movement in degrees
     * @param distance Distance to move
     * @return Reference to this line
     */
    Line& move(double bearing, double distance);

    /**
     * @brief Move this line in a cardinal direction by a distance.
     * @param direction Cardinal direction
     * @param distance Distance to move
     * @return Reference to this line
     */
    Line& move(Geo::Direction direction, double distance);

    /**
     * @brief Translate this line by a (dx, dy) delta.
     * @param dx Horizontal displacement
     * @param dy Vertical displacement
     * @return Reference to this line
     */
    Line& moveDelta(double dx, double dy);

    /**
     * @brief Rotate this line around a centroid by an angle.
     * @param centroid Centre of rotation
     * @param angle Rotation angle in degrees
     * @return Reference to this line
     */
    Line &rotate(const Point& centroid, double angle);

    /** @brief Convert to a QLine (integer endpoints).
     * @return QLine equivalent */
    QLine toQLine() const;
    /** @brief Convert to a QLineF (floating-point endpoints).
     * @return QLineF equivalent */
    QLineF toQLineF() const;

    /** @brief Format this line as a human-readable string.
     * @return String representation */
    QString toString() const;

    /**
     * @brief Test whether this line has non-zero length.
     * @return true if length() != 0
     */
    bool isValid() const { return length() != 0; }

private:
    Point _p1;
    Point _p2;

    /** @brief Test whether a value lies between v1 and v2 (inclusive). */
    static inline bool isBetween(double value, double v1, double v2)
    {
        return value >= v1 && value <= v2;
    }
};

#endif // LINE_H
