#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPointF>
#include <Kanoop/kanoopcommon.h>
#include <Kanoop/geometry/point.h>

class Line;
class KANOOP_EXPORT Circle
{
public:
    Circle() : _radius(0) {}
    Circle(const QPointF center, double radius) :
        _center(center), _radius(radius) {}
    static Circle fromThreePoints(const Point& a, const Point& b, const Point& c);

    QPointF center() const { return _center; }
    void setCenter(const QPointF& value) { _center = value; }

    double radius() const { return _radius; }
    void setRadius(double value) { _radius = value; }

    double circumference() const;
    double diameter() const;
    double area() const;

    bool intersects(const Line& line) const;
    int intersects(const Line& line, QPointF& intersection1, QPointF& intersection2) const;

    QString toString() const;

private:
    static Point findIntersection(double m1, double b1, double m2, double b2);

    Point _center;
    double _radius;
};

#endif // CIRCLE_H
