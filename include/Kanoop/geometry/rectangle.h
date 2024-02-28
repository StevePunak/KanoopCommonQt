#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <QMap>
#include <QRectF>
#include "point.h"
#include "line.h"
#include "Kanoop/kanoopcommon.h"

class KANOOP_EXPORT Rectangle : public QRectF
{
public:
    Rectangle() : QRectF() {}
    Rectangle(const QRectF& other) : QRectF(other) {}
    Rectangle(const QRect& other) : QRectF(other) {}
    Rectangle(const QPointF& topLeft, const QSizeF& size) : QRectF(topLeft, size) {}
    Rectangle(double x, double y, int w, int h) : QRectF(x, y, w, h) {}

    static Rectangle fromPoints(const Point::List& points);
    static Rectangle fromPoints(const Point& p1, const Point& p2);
    static Rectangle fromCenterLine(const Line& centerLine, double expand);
    static Rectangle fromCenterPoint(const Point& centerPoint, double expand);

    Point closestCorner(const Point& origin) const;
    Point::List corners() const;
    Line::List edges() const;
    QMap<Geo::Side, Line> edgeMap() const;
    Line::List verticalLines() const;
    Line::List horizontalLines() const;
    Line topEdge() const;
    Line bottomEdge() const;
    Line leftEdge() const;
    Line rightEdge() const;
    Line closestEdge(const Point& point);
    bool isPointOnEdge(const Point& point, Line &foundEdge) const;
    bool isPointOnEdge(const Point& point) const;
    bool containsAnyPoint(const Line& line) const;
    Point centerPoint() const;
    Geo::Side closestSideToPoint(const Point &point) const;
    void scale(double scale);
    Rectangle scaled(double scale) const;

    QString toString() const { return QString("x,y %1, %2  w,h %3, %4").arg(x()).arg(y()).arg(width()).arg(height()); }

    class List : public QList<Rectangle> {};

};

#endif // RECTANGLE_H
