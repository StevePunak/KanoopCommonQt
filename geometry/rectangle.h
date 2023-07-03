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

    Point closestCorner(const Point& origin) const;
    Line::List lines() const;
    Line::List verticalLines() const;
    Line::List horizontalLines() const;

};

#endif // RECTANGLE_H
