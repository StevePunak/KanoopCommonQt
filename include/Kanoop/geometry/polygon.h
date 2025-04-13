#ifndef POLYGON_H
#define POLYGON_H
#include "point.h"
#include "line.h"

class KANOOP_EXPORT Polygon
{
public:
    Polygon() {}

    static Polygon fromLine(const Line& line, int expandBy);

    void appendPoint(const Point& point) { _points.append(point); }

    double area() const;
    void move(double bearing, double distance);
    void rotate(const Point& centroid, double angle);
    Point centroid() const;
    bool contains(const Point& point) const;
    Line::List lines() const;
    Rectangle boundingRectangle() const;

    bool isValid() const { return _points.count() > 2; }

private:
    void createFromLines(const Line::List &lines);
    double minimumXY() const;

    Point::List _points;
};

#endif // POLYGON_H
