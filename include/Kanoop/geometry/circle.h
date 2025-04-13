#ifndef CIRCLE_H
#define CIRCLE_H

#include <QPointF>
#include "Kanoop/kanoopcommon.h"

class Line;
class KANOOP_EXPORT Circle
{
public:
    Circle() : _radius(0) {}
    Circle(const QPointF center, double radius) :
        _center(center), _radius(radius) {}

    QPointF center() const { return _center; }
    void setCenter(const QPointF& value) { _center = value; }

    double radius() const { return _radius; }
    void setRadius(double value) { _radius = value; }

    double circumference() const;
    double diameter() const;
    double area() const;

    bool intersects(const Line& line) const;
    int intersects(const Line& line, QPointF& intersection1, QPointF& intersection2) const;

private:
    QPointF _center;
    double _radius;
};

#endif // CIRCLE_H
