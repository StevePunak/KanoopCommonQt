#ifndef POINT_H
#define POINT_H
#include <QList>
#include <QPointF>

#include "geo.h"

class Point : public QPointF
{
public:
    Point() : QPointF() {}
    Point(const QPointF& other) : QPointF(other) {}
    Point(const QPoint& other) : QPointF(other) {}
    Point(double x, double y) : QPointF(x, y) {}

    bool isLeftOf(const QPointF& other) const { return x() < other.x(); }
    bool isRightOf(const QPointF& other) const { return x() > other.x(); }
    bool isAbove(const QPointF& other) const { return y() < other.y(); }
    bool isBelow(const QPointF& other) const { return y() > other.y(); }

    Point& move(Geo::Direction direction, double amount);
    Point& move(double bearing, double distance);
    Point& round();
    Point& offset(double x, double y);

    QString toString() const { return QString("%1, %2").arg(x()).arg(y()); }
    static Point fromString(const QString& value);

    class List : public QList<Point>
    {
    public:
        Point topLeft() const;
        Point topRight() const;
        Point bottomLeft() const;
        Point bottomRight() const;
    };
};

#endif // POINT_H
