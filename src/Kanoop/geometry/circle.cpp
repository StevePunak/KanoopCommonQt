#include "Kanoop/geometry/circle.h"
#include "Kanoop/geometry/line.h"
#include <QtMath>
#include <stringutil.h>

Circle Circle::fromThreePoints(const Point& a, const Point& b, const Point& c)
{
    // calculate midpoints
    Point midpointAB = Line(a, b).midpoint();
    Point midpointAC = Line(a, c).midpoint();

    // Calculate slopes
    double slopeAB = Line(a, b).slope();
    double slopeAC = Line(a, c).slope();

    // Caclulate perpendicular slopes
    double perpendicularSlopeAB = -1 / slopeAB;
    double perpendicularSlopeAC = -1 / slopeAC;

    // Calculate y-intercepts for perpendicular bisectors
    double bisectorAB_b = midpointAB.y() - perpendicularSlopeAB * midpointAB.x();
    double bisectorAC_b = midpointAC.y() - perpendicularSlopeAC * midpointAC.x();

    // Find the intersection point (circle center)
    Point origin = findIntersection(perpendicularSlopeAB, bisectorAB_b, perpendicularSlopeAC, bisectorAC_b);

    Circle result(origin, FlatGeo::distance(origin, a));
    return result;
}

double Circle::circumference() const
{
    return 2 * M_PI * _radius;
}

double Circle::diameter() const
{
    return _radius * 2;
}

double Circle::area() const
{
    return M_PI * (_radius * _radius);
}

bool Circle::intersects(const Line &line) const
{
    QPointF i1, i2;
    return intersects(line, i1, i2) > 0;
}

int Circle::intersects(const Line &line, QPointF &intersection1, QPointF &intersection2) const
{
    int		intersections = 0;

    double	dx, dy, A, B, C, det, t;

    dx = line.p2().x() - line.p1().x();
    dy = line.p2().y() - line.p1().y();

    A = dx * dx + dy * dy;
    B = 2 * (dx * (line.p1().x() - _center.x()) + dy * (line.p1().y() - _center.y()));
    C = (line.p1().x() - _center.x()) * (line.p1().x() - _center.x()) + (line.p1().y() - _center.y()) * (line.p1().y() - _center.y()) - _radius * _radius;

    det = B * B - 4 * A * C;

    if((A <= 0.0000001) || (det < 0))
    {
        // No real solutions.
    }
    else if(det == 0)
    {
        // One solution.
        t = -B / (2 * A);
        intersection1 = QPointF(line.p1().x() + t * dx, line.p1().y() + t * dy);
        intersections = 1;
    }
    else
    {
        // Two solutions.
        t = (float)((-B + qSqrt(det)) / (2 * A));
        intersection1 = QPointF(line.p1().x() + t * dx, line.p1().y() + t * dy);
        t = (float)((-B - qSqrt(det)) / (2 * A));
        intersection2 = QPointF(line.p1().x() + t * dx, line.p1().y() + t * dy);
        intersections = 2;
    }
    return intersections;
}

QString Circle::toString() const
{
    return QString("c: %1  r: %2").arg(_center.toString()).arg(StringUtil::toString(_radius));
}

Point Circle::findIntersection(double m1, double b1, double m2, double b2)
{
    Point intersection;
    intersection.setX((b2 - b1) / (m1 - m2));
    intersection.setY(m1 * intersection.x() + b1);
    return intersection;
}
