#include "polygon.h"
#include <limits>
#include "angle.h"
#include "rectangle.h"

Polygon Polygon::fromLine(const Line &line, int expandBy)
{
    Angle angle = line.angle();
    angle += 90;
    Line l1 = line;
    l1.move(angle.degrees(), expandBy);
    l1.grow(expandBy);

    angle += 180;
    Line l2 = line;
    l2.move(angle.degrees(), expandBy);
    l2.grow(expandBy);

    Polygon result;
    result._points.append(l1.p1());
    result._points.append(l2.p1());
    result._points.append(l2.p2());
    result._points.append(l1.p2());
    return result;
}

double Polygon::area() const
{
    int		i = 0;
    int		j = _points.count() - 1;
    double	area = 0;

    for (i = 0; i < _points.count(); j = i++) {
        area += _points[i].x() * _points[j].y();
        area -= _points[i].y() * _points[j].x();
    }
    area /= 2.0;

    return area;
}

void Polygon::move(double bearing, double distance)
{
    for(Point& point : _points) {
        point.move(bearing, distance);
    }
}

void Polygon::rotate(const Point &centroid, double angle)
{
    Line::List lineList = lines();

    for(Line& line : lineList) {
        line.rotate(centroid, angle);
    }

    createFromLines(lineList);
}

Point Polygon::centroid() const
{
    Point	centroid;
    double	signedArea = 0.0;
    double	x0 = 0.0;	// Current vertex X
    double	y0 = 0.0;	// Current vertex Y
    double	x1 = 0.0;	// Next vertex X
    double	y1 = 0.0;	// Next vertex Y
    double	a = 0.0;// Partial signed area

    // For all vertices except last
    int		i = 0;

    for (i = 0; i < _points.count() - 1; ++i) {
        x0 = _points[i].x();
        y0 = _points[i].y();
        x1 = _points[i + 1].x();
        y1 = _points[i + 1].y();
        a = x0 * y1 - x1 * y0;
        signedArea += a;
        centroid.rx() += (x0 + x1) * a;
        centroid.ry() += (y0 + y1) * a;
    }

    // Do last vertex
    x0 = _points[i].x();
    y0 = _points[i].y();
    x1 = _points[0].x();
    y1 = _points[0].y();
    a = x0 * y1 - x1 * y0;
    signedArea += a;
    centroid.rx() += (x0 + x1) * a;
    centroid.ry() += (y0 + y1) * a;

    signedArea *= 0.5;
    centroid.rx() /= (6 * signedArea);
    centroid.ry() /= (6 * signedArea);

    return centroid;
}

bool Polygon::contains(const Point &point) const
{
    if(!isValid()) {
        return false;
    }

    Point p1, p2;
    bool inside = false;

    Point::List	poly = _points;

    Point oldPoint = poly[_points.count() - 1];

    for (int i = 0; i < poly.count(); i++) {
        Point newPoint = poly[i];
        if (newPoint.x() > oldPoint.x()) {
            p1 = oldPoint;
            p2 = newPoint;
        }
        else {
            p1 = newPoint;
            p2 = oldPoint;
        }

        if ((newPoint.x() < point.x()) == (point.x() <= oldPoint.x()) &&
            (point.y() - (long)p1.y()) * (p2.x() - p1.x()) < (p2.y() - (long)p1.y()) * (point.x() - p1.x())) {
            inside = !inside;
        }

        oldPoint = newPoint;
    }

    return inside;
}

Line::List Polygon::lines() const
{
    Line::List result;
    if(_points.count() > 0)
    {
        Point previous;
        bool first = true;

        for(const Point& point : _points) {
            if(first == false) {
                result.append(Line(previous, point));
            }
            previous = point;
            first = false;
        }
        result.append(Line(previous, _points.first()));
    }
    return result;
}

Rectangle Polygon::boundingRectangle() const
{
    double leftMost = std::numeric_limits<double>::max();
    double rightMost = std::numeric_limits<double>::min();
    double topMost = std::numeric_limits<double>::max();
    double bottomMost = std::numeric_limits<double>::min();

    for(const Line& line : lines()) {
        for(const Point& point : line.points()) {
            if(point.x() < leftMost)
                leftMost = point.x();
            if(point.x() > rightMost)
                rightMost = point.x();
            if(point.y() < topMost)
                topMost = point.y();
            if(point.y() > bottomMost)
                bottomMost = point.y();
        }
    }
    return Rectangle(leftMost, topMost, rightMost - leftMost, bottomMost - topMost);
}

void Polygon::createFromLines(const Line::List& lines)
{
    _points.clear();
    for(const Line& line : lines) {
        _points.append(line.p1());
    }
}

double Polygon::minimumXY() const
{
    double min = INFINITY;
    for(const Point& point : _points) {
        if(point.x() < min) {
            min = point.x();
        }
        if(point.y() < min) {
            min = point.y();
        }
    }
    return min;
}
