#ifndef LINE_H
#define LINE_H

#include <QList>
#include <QPoint>
#include <QRect>

#include "flatgeo.h"
#include "point.h"

class QLineF;
class QLine;
class Circle;
class Line
{
public:
    Line() {}
    Line(const QPoint& p1, const QPoint& p2) :
        _p1(p1), _p2(p2) {}
    Line(const QPointF& p1, const QPointF& p2) :
        _p1(p1), _p2(p2) {}
    Line(const QPointF& origin, double bearing, double distance);

    bool operator ==(const Line& other) const { return _p1 == other._p1 && _p2 == other._p2; }
    bool operator !=(const Line& other) const { return (*this == other) == false; }

    QPointF p1() const { return _p1; }
    QPointF p2() const { return _p2; }

    class List : public QList<Line>
    {
    public:
        static List fromPoints(const QList<QPoint>& points);
        static List fromPoints(const QList<QPointF>& points);
        Line longestHorizontalLine();
        Line longestVerticalLine();
        bool containsLineWithSameEndpoints(const Line& line) const;
        Line highest() const;
        Line lowest() const;
        QPointF closestPointTo(const QPointF& other, Line& closestLine, double &closestDistance);
    };

    QPointF midpoint() const;
    double length() const;
    double slope() const;
    double intercept() const;
    bool isVertical() const;
    bool isHorizontal() const;
    double bearing() const;
    double distanceTo(const QPointF& to) const;
    QPointF closestPointTo(const QPointF& point) const;
    QPointF closestPointTo(const QPointF& point, double& distance) const;
    QPointF furthestPointFrom(const QPointF& point) const;
    QPointF topMostPoint() const;
    QPointF bottomMostPoint() const;
    QPointF leftMostPoint() const;
    QPointF rightMostPoint() const;
    bool hasXBetween(double x1, double x2) const;
    bool crossesX(double x) const;
    bool hasYBetween(double y1, double y2) const;
    bool crossesY(double y) const;
    bool intersects(const Line& other) const;
    bool intersects(const Line& other, QPointF& intersection) const;
    bool intersects(const Circle& other) const;
    bool intersects(const QRectF &other) const;
    QPointF intersection(const Line& other) const;
    bool isLeftOf(const Line& other) const;
    bool isRightOf(const Line& other) const;
    bool isAbove(const Line& other) const;
    bool isBelow(const Line& other) const;
    bool sharesEndpointWith(const Line& other, double maxDistance = 0) const;
    bool sharesSameEndpoints(const Line& other) const;
    bool isEndpoint(const QPointF& point, int precision = 0) const;
    bool containsPoint(const QPointF& point) const;

    static List verticalLines(const QRectF& rect);
    static List horizontalLines(const QRectF& rect);

    void move(double bearing, double distance);
    void rotate(const QPointF& centroid, double angle);

    QLine toQLine() const;
    QLineF toQLineF() const;

    QString toString() const;

    bool isValid() const { return _p1.isNull() == false && _p2.isNull() == false; }

private:
    QPointF _p1;
    QPointF _p2;

    static inline bool isBetween(double value, double v1, double v2)
    {
        return value >= v1 && value <= v2;
    }
};

#endif // LINE_H
