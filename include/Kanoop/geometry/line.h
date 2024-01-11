#ifndef LINE_H
#define LINE_H

#include <QList>
#include <QPoint>
#include <QRect>

#include "Kanoop/kanoopcommon.h"
#include "flatgeo.h"
#include "point.h"

class QLineF;
class QLine;
class Circle;
class Rectangle;
class KANOOP_EXPORT Line
{
public:
    Line() {}
    Line(const QPoint& p1, const QPoint& p2) :
        _p1(p1), _p2(p2) {}
    Line(const QPointF& p1, const QPointF& p2) :
        _p1(p1), _p2(p2) {}
    Line(const QPointF& origin, double bearing, double distance);
    Line(const QPointF& origin, Geo::Direction direction, double distance);

    bool operator ==(const Line& other) const { return _p1 == other._p1 && _p2 == other._p2; }
    bool operator !=(const Line& other) const { return (*this == other) == false; }

    Point p1() const { return _p1; }
    void setP1(const Point& value) { _p1 = value; }

    Point p2() const { return _p2; }
    void setP2(const Point& value) { _p2 = value; }

    class KANOOP_EXPORT List : public QList<Line>
    {
    public:
        List() : QList<Line>() {}
        List(const QList<Line>& other) {
            for(const Line& line : other) {
                append(line);
            }
        }

        static List fromPoints(const QList<QPoint>& points);
        static List fromPoints(const QList<QPointF>& points);
        Line longestHorizontalLine();
        Line longestVerticalLine();
        bool containsLineWithSameEndpoints(const Line& line) const;
        Line highest() const;
        Line lowest() const;
        Line shortest() const;
        Line longest() const;
        QPointF closestPointTo(const QPointF& other);
        QPointF closestPointTo(const QPointF& other, Line& closestLine, double &closestDistance);
        double minX() const;
        double maxX() const;
        double minY() const;
        double maxY() const;
        Rectangle boundingRectangle() const;
        Line lineContainingPoint(const Point& point) const;
        Line lineNearPoint(const Point& point, int margin) const;
        double totalLength() const;
        List &moveDelta(double dx, double dy);

        QList<QLineF> toQLineFList() const;
        QList<QLine> toQLineList() const;

        QString toString() const;
    };

    Point midpoint() const;
    double length() const;
    double slope() const;
    double intercept() const;
    bool isVertical() const;
    bool isHorizontal() const;
    double bearing() const;
    Angle angle() const;
    double distanceTo(const QPointF& to) const;
    Point closestPointTo(const QPointF& point) const;
    Point closestPointTo(const QPointF& point, double& distance) const;
    Point furthestPointFrom(const QPointF& point) const;
    Point topMostPoint() const;
    Point bottomMostPoint() const;
    Point leftMostPoint() const;
    Point rightMostPoint() const;
    bool hasXBetween(double x1, double x2) const;
    bool crossesX(double x) const;
    bool hasYBetween(double y1, double y2) const;
    bool crossesY(double y) const;
    bool intersects(const Line& other) const;
    bool intersects(const Line& other, Point &intersection) const;
    bool intersects(const Circle& other) const;
    bool intersects(const QRectF &other) const;
    Point intersection(const Line& other) const;
    bool isLeftOf(const Line& other) const;
    bool isRightOf(const Line& other) const;
    bool isAbove(const Line& other) const;
    bool isBelow(const Line& other) const;
    bool isPerpendicular() const;
    bool sharesAxisWith(const Line& other) const;
    bool sharesEndpointWith(const Line& other, double maxDistance = 0) const;
    bool sharesSameEndpoints(const Line& other) const;
    bool isEndpoint(const QPointF& point, int precision = 0) const;
    bool containsPoint(const QPointF& point) const;
    Line& shorten(double howMuch);
    Line& extend(double howMuch);
    Line& grow(double howMuch);
    Geo::Direction direction() const;
    double minX() const;
    double maxX() const;
    double minY() const;
    double maxY() const;
    Point::List points() const;

    Line& round();

    Rectangle makeRectangle(int expandedWidth) const;

    static List verticalLines(const QRectF& rect);
    static List horizontalLines(const QRectF& rect);

    Line& move(double bearing, double distance);
    Line& move(Geo::Direction direction, double distance);
    Line& moveDelta(double dx, double dy);
    Line &rotate(const Point& centroid, double angle);

    QLine toQLine() const;
    QLineF toQLineF() const;

    QString toString() const;

    bool isValid() const { return length() != 0; }

private:
    Point _p1;
    Point _p2;

    static inline bool isBetween(double value, double v1, double v2)
    {
        return value >= v1 && value <= v2;
    }
};

#endif // LINE_H
