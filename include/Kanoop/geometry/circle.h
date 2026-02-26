/**
 * @brief A 2D circle defined by a center point and radius.
 */
#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPointF>
#include <Kanoop/kanoopcommon.h>
#include <Kanoop/geometry/point.h>

class Line;

/**
 * @brief Represents a 2D circle with centre and radius, supporting geometric queries.
 */
class KANOOP_EXPORT Circle
{
public:
    /** @brief Default constructor — creates a zero-radius circle at the origin. */
    Circle() : _radius(0) {}

    /**
     * @brief Construct a circle from a centre point and radius.
     * @param center Centre of the circle
     * @param radius Radius of the circle
     */
    Circle(const QPointF center, double radius) :
        _center(center), _radius(radius) {}

    /**
     * @brief Construct the unique circle passing through three non-collinear points.
     * @param a First point on the circle
     * @param b Second point on the circle
     * @param c Third point on the circle
     * @return Circumscribed circle, or an invalid circle if points are collinear
     */
    static Circle fromThreePoints(const Point& a, const Point& b, const Point& c);

    /**
     * @brief Return the centre of the circle.
     * @return Centre QPointF
     */
    QPointF center() const { return _center; }

    /**
     * @brief Set the centre of the circle.
     * @param value New centre point
     */
    void setCenter(const QPointF& value) { _center = value; }

    /**
     * @brief Return the radius of the circle.
     * @return Radius value
     */
    double radius() const { return _radius; }

    /**
     * @brief Set the radius of the circle.
     * @param value New radius value
     */
    void setRadius(double value) { _radius = value; }

    /**
     * @brief Return the circumference of the circle.
     * @return Circumference (2 * pi * radius)
     */
    double circumference() const;

    /**
     * @brief Return the diameter of the circle.
     * @return Diameter (2 * radius)
     */
    double diameter() const;

    /**
     * @brief Return the area of the circle.
     * @return Area (pi * radius^2)
     */
    double area() const;

    /**
     * @brief Test whether a line intersects this circle.
     * @param line Line segment to test
     * @return true if the line intersects or is tangent to the circle
     */
    bool intersects(const Line& line) const;

    /**
     * @brief Find the intersection points of a line with this circle.
     * @param line Line segment to intersect
     * @param intersection1 Output for the first intersection point
     * @param intersection2 Output for the second intersection point
     * @return Number of intersection points (0, 1, or 2)
     */
    int intersects(const Line& line, QPointF& intersection1, QPointF& intersection2) const;

    /**
     * @brief Format the circle as a human-readable string.
     * @return String representation of centre and radius
     */
    QString toString() const;

private:
    /** @brief Compute the intersection of two lines given in slope-intercept form. */
    static Point findIntersection(double m1, double b1, double m2, double b2);

    Point _center;
    double _radius;
};

#endif // CIRCLE_H
