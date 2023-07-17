#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <QRectF>
#include "point.h"
#include "line.h"

class Rectangle : public QRectF
{
public:
    Rectangle() : QRectF() {}
    Rectangle(const QRectF& other) : QRectF(other) {}
    Rectangle(const QRect& other) : QRectF(other) {}
    Rectangle(const QPointF& topLeft, const QSizeF& size) : QRectF(topLeft, size) {}
    Rectangle(double x, double y, int w, int h) : QRectF(x, y, w, h) {}

    static Rectangle fromPoints(const Point::List& points);

    Point closestCorner(const Point& origin) const;
    Point::List corners() const;
    Line::List edges() const;
    Line::List verticalLines() const;
    Line::List horizontalLines() const;
    Line upperEdge() const;
    Line lowerEdge() const;
    Line leftEdge() const;
    Line rightEdge() const;
    bool isPointOnEdge(const Point& point) const;
    bool containsAnyPoint(const Line& line) const;
    Point centerPoint() const;

    QString toString() const { return QString("x,y %1, %2  w,h %3, %4").arg(x()).arg(y()).arg(width()).arg(height()); }

    class List : public QList<Rectangle> {};

};

#endif // RECTANGLE_H
