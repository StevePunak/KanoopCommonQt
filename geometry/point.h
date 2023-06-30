#ifndef POINT_H
#define POINT_H
#include <QPointF>

class Point : public QPointF
{
public:
    Point() : QPointF() {}
    Point(const QPointF& other) : QPointF(other) {}
    Point(const QPoint& other) : QPointF(other) {}
    Point(double x, double y) : QPointF(x, y) {}

    bool isLeftOf(const QPointF& other) { return x() < other.x(); }
    bool isRightOf(const QPointF& other) { return x() > other.x(); }
    bool isAbove(const QPointF& other) { return y() < other.y(); }
    bool isBelow(const QPointF& other) { return y() > other.y(); }
};

#endif // POINT_H
