#include "angle.h"
#include "circle.h"
#include "flatgeo.h"
#include "line.h"
#include "polygon.h"
#include "rectangle.h"

#include <QLine>
#include <QLineF>
#include <QTextStream>
#include <QtMath>
#include <float.h>

Line::Line(const QPointF &origin, double bearing, double distance) :
    _p1(origin), _p2(FlatGeo::getPoint(origin, bearing, distance)) {}

Line::Line(const QPointF &origin, Geo::Direction direction, double distance) :
    _p1(origin), _p2(FlatGeo::getPoint(origin, Geo::directionToBearing(direction), distance)) {}

Point Line::midpoint() const
{
    double x = _p2.x() - ((_p2.x() - _p1.x()) / 2);
    double y = _p2.y() - ((_p2.y() - _p1.y()) / 2);
    return QPointF(x, y);
}

double Line::length() const
{
    double	x = _p1.x() - _p2.x();
    double	y = _p1.y() - _p2.y();
    return(qSqrt((x*x) + (y*y)));
}

double Line::slope() const
{
    double	result = 0;
    double	y = _p2.y() - _p1.y();
    double	x = _p2.x() - _p1.x();
    if(y == 0) {
        result = DBL_MAX;
    }
    else if(x == 0) {
        result = DBL_MIN;
    }
    else {
        result = y / x;
    }
    return result;

}

double Line::intercept() const
{
    double ret = DBL_MAX;
    double theSlope = slope();

    /** not vertical */
    if(ret != DBL_MIN) {
        ret = _p1.y() - (_p1.x() * theSlope);
    }
    return ret;
}

bool Line::isVertical() const
{
    return _p1.x() == _p2.x();
}

bool Line::isHorizontal() const
{
    return _p1.y() == _p2.y();
}

double Line::bearing() const
{
    /**
     * get a new line and move it down one unit
     */
    Line newLine = *this;
    newLine.move(180.0, 1);

    Line l(newLine.p1(), QPointF(newLine._p1.x(), newLine._p1.y() - 1));
    double d = FlatGeo::vectorAngle(l, newLine);
    Angle angle(d);
    if(_p2.x() < _p1.x()) {
        angle.setDegrees(360 - angle.degrees());
    }

    return angle.degrees();
}

Angle Line::angle() const
{
    return Angle(bearing());
}

double Line::distanceTo(const QPointF &to) const
{
    double	result = DBL_MAX;

    /** make a single leg of a triangle between the remote point and one point on this line */
    Line leg(_p1, to);

    /** get the interior angle */
    double	angle = FlatGeo::angle(to, leg._p1, leg._p2).degrees();
    if(angle >= 180) {
        angle -= 180;
    }

    if(angle != 0) {
        /** find the distance to the closest point on the line */
        result = qFabs(qSin(angle) * leg.length());
    }
    else {
        /** the point is on the line */
        result = 0;
    }

    return result;
}

Point Line::closestPointTo(const QPointF &point) const
{
    double distance;
    return closestPointTo(point, distance);
}

Point Line::closestPointTo(const QPointF &point, double &distance) const
{
    QPointF	closest;
    double dx = _p2.x() - _p1.x();
    double dy = _p2.y() - _p1.y();

    // Calculate the t that minimizes the distance.
    double t = ((point.x() - _p1.x()) * dx + (point.y() - _p1.y()) * dy) / (dx * dx + dy * dy);

    // See if this represents one of the segment's
    // end points or a point in the middle.
    if (t < 0) {
        closest = QPointF(_p1.x(), _p1.y());
        dx = point.x() - _p1.x();
        dy = point.y() - _p1.y();
    }
    else if (t > 1) {
        closest = QPointF(_p2.x(), _p2.y());
        dx = point.x() - _p2.x();
        dy = point.y() - _p2.y();
    }
    else
    {
        closest = QPointF(_p1.x() + t * dx, _p1.y() + t * dy);
        dx = point.x() - closest.x();
        dy = point.y() - closest.y();
    }

    distance = qSqrt(dx * dx + dy * dy);

    return closest;
}

Point Line::furthestPointFrom(const QPointF &point) const
{
    double distance;
    QPointF closest = closestPointTo(point, distance);
    QPointF furthest = closest == _p1 ? _p2 : _p1;
    return furthest;
}

Point Line::topMostPoint() const
{
    return FlatGeo::isPointAbove(_p1, _p2) ? _p1 : _p2;
}

Point Line::bottomMostPoint() const
{
    return FlatGeo::isPointBelow(_p1, _p2) ? _p1 : _p2;
}

Point Line::leftMostPoint() const
{
    return FlatGeo::isPointLeftOf(_p1, _p2) ? _p1 : _p2;
}

Point Line::rightMostPoint() const
{
    return FlatGeo::isPointRightOf(_p1, _p2) ? _p1 : _p2;
}

bool Line::hasXBetween(double x1, double x2) const
{
    return isBetween(_p1.x(), x1, x2) || isBetween(_p2.x(), x1, x2);
}

bool Line::crossesX(double x) const
{
    return isBetween(x, _p1.x(), _p2.x());
}

bool Line::hasYBetween(double y1, double y2) const
{
    return isBetween(_p1.y(), y1, y2) || isBetween(_p2.y(), y1, y2);
}

bool Line::crossesY(double y) const
{
    return isBetween(y, _p1.y(), _p2.y());
}

bool Line::intersects(const Line &other) const
{
    QPointF point = intersection(other);
    return point.isNull() == false;
}

bool Line::intersects(const Line &other, Point &intersection) const
{
    intersection = FlatGeo::intersection(*this, other);
    return intersection.isNull() == false;
}

bool Line::intersects(const Circle &other) const
{
    return other.intersects(*this);
}

bool Line::intersects(const QRectF &other) const
{
    return intersects(Line(other.topLeft(), other.topRight())) ||
           intersects(Line(other.topRight(), other.bottomRight())) ||
           intersects(Line(other.bottomRight(), other.bottomLeft())) ||
           intersects(Line(other.bottomLeft(), other.topLeft()));
}

Point Line::intersection(const Line &other) const
{
    return FlatGeo::intersection(*this, other);
}

bool Line::isLeftOf(const Line &other) const
{
    double minL1 = qMin(_p1.x(), _p2.x());
    double minL2 = qMin(other._p1.x(), other._p2.x());

    return minL1 < minL2;
}

bool Line::isRightOf(const Line &other) const
{
    double maxL1 = qMax(_p1.x(), _p2.x());
    double maxL2 = qMax(other._p1.x(), other._p2.x());

    return maxL1 > maxL2;
}

bool Line::isAbove(const Line &other) const
{
    double minL1 = qMin(_p1.y(), _p2.y());
    double minL2 = qMin(other._p1.y(), other._p2.y());

    return minL1 < minL2;

}

bool Line::isBelow(const Line &other) const
{
    double maxL1 = qMax(_p1.y(), _p2.y());
    double maxL2 = qMax(other._p1.y(), other._p2.y());

    return maxL1 > maxL2;
}

bool Line::isPerpendicular() const
{
    return isVertical() || isHorizontal();
}

/**
 * @brief Line::sharesAxisWith
 * @param other
 * @return true if y or x is the same depending on the line direction
 */
bool Line::sharesAxisWith(const Line &other) const
{
    bool result = false;
    if(isVertical() && other.isVertical()) {
        result = _p1.x() == other._p1.x();
    }
    else if(isHorizontal() && other.isHorizontal()) {
        result = _p1.y() == other._p1.y();
    }
    return result;
}

bool Line::sharesEndpointWith(const Line &other, double maxDistance) const
{
    bool result =
                  FlatGeo::distance(_p1, other._p1) <= maxDistance ||
                  FlatGeo::distance(_p2, other._p2) <= maxDistance ||
                  FlatGeo::distance(_p1, other._p2) <= maxDistance ||
                  FlatGeo::distance(_p2, other._p1) <= maxDistance;
    return result;
}

bool Line::sharesSameEndpoints(const Line &other) const
{
    return (_p1 == other._p1 && _p2 == other._p2) ||
           (_p1 == other._p2 && _p2 == other._p1);
}

bool Line::isEndpoint(const QPointF &point, int precision) const
{
    return FlatGeo::arePointsEqual(_p1, point, precision) || FlatGeo::arePointsEqual(_p2, point, precision);
}

bool Line::containsPoint(const QPointF &point) const
{
    double d1 = FlatGeo::distance(_p1, point);
    double d2 = FlatGeo::distance(_p2, point);
    return d1 + d2 == length();
}

Line &Line::shorten(double howMuch)
{
    if(length() >= howMuch) {
        Point newP2 = FlatGeo::getPoint(_p1, bearing(), length() - howMuch);
        _p2 = newP2;
    }
    return *this;
}

Line &Line::extend(double howMuch)
{
    Point newP2 = FlatGeo::getPoint(_p1, bearing(), length() + howMuch);
    _p2 = newP2;
    return *this;
}

Geo::Direction Line::direction() const
{
    return Geo::bearingToDirection(qRound(bearing()));
}

double Line::minX() const
{
    return qMin(_p1.x(), _p2.x());
}

double Line::maxX() const
{
    return qMax(_p1.x(), _p2.x());
}

double Line::minY() const
{
    return qMin(_p1.y(), _p2.y());
}

double Line::maxY() const
{
    return qMax(_p1.y(), _p2.y());
}

Point::List Line::points() const
{
    Point::List result;
    result.append(_p1);
    result.append(_p2);
    return result;
}

Line &Line::round()
{
    _p1.round();
    _p2.round();
    return *this;
}

Rectangle Line::makeRectangle(int expandedWidth) const
{
    Rectangle result;
    if(isVertical()) {
        int tx = topMostPoint().x() - expandedWidth;
        int ty = topMostPoint().y() - expandedWidth;
        int w = expandedWidth * 2;
        int h = length() + expandedWidth * 2;

        result = Rectangle(tx, ty, w, h);
    }
    else {
        int tx = leftMostPoint().x() - expandedWidth;
        int ty = leftMostPoint().y() - expandedWidth;
        int w = length() + expandedWidth * 2;
        int h = expandedWidth * 2;

        result = Rectangle(tx, ty, w, h);
    }
    return result;
}

Line::List Line::verticalLines(const QRectF &rect)
{
    List result;
    result.append(Line(rect.topLeft(), rect.bottomLeft()));
    result.append(Line(rect.topRight(), rect.bottomRight()));
    return result;
}

Line::List Line::horizontalLines(const QRectF &rect)
{
    List result;
    result.append(Line(rect.topLeft(), rect.topRight()));
    result.append(Line(rect.bottomLeft(), rect.bottomRight()));
    return result;
}

Line &Line::move(double bearing, double distance)
{
    _p1 = FlatGeo::move(_p1, bearing, distance);
    _p2 = FlatGeo::move(_p2, bearing, distance);
    return *this;
}

Line &Line::move(Geo::Direction direction, double distance)
{
    return move(Geo::directionToBearing(direction), distance);
}

Line &Line::moveDelta(double dx, double dy)
{
    _p1.moveDelta(dx, dy);
    _p2.moveDelta(dx, dy);
    return *this;
}

Line&  Line::rotate(const Point &centroid, double angle)
{
    _p1 = FlatGeo::rotate(_p1, centroid, angle);
    _p2 = FlatGeo::rotate(_p2, centroid, angle);
    return *this;
}

QLine Line::toQLine() const
{
    return QLine(_p1.toPoint(), _p2.toPoint());
}

QLineF Line::toQLineF() const
{
    return QLineF(_p1, _p2);
}

QString Line::toString() const
{
    return QString("p1: %1  p2: %2").arg(FlatGeo::makePointString(_p1)).arg(FlatGeo::makePointString(_p2));
}


// ======================== Line::List =============================

Line::List Line::List::fromPoints(const QList<QPoint>& points)
{
    List result;
    for(int i = 0;i < points.count() - 1;i++) {
        result.append(Line(points.at(i), points.at(i+1)));
    }
    return result;
}

Line::List Line::List::fromPoints(const QList<QPointF>& points)
{
    List result;
    for(int i = 0;i < points.count() - 1;i++) {
        result.append(Line(points.at(i), points.at(i+1)));
    }
    return result;
}

Line Line::List::longestHorizontalLine()
{
    Line result;
    for(const Line& line : *this) {
        if(line.isHorizontal() && line.length() > result.length()) {
            result = line;
        }
    }
    return result;
}

Line Line::List::longestVerticalLine()
{
    Line result;
    for(const Line& line : *this) {
        if(line.isVertical() && line.length() > result.length()) {
            result = line;
        }
    }
    return result;
}

bool Line::List::containsLineWithSameEndpoints(const Line& line) const
{
    bool result = false;
    const_iterator it = std::find_if(constBegin(), constEnd(), [line](const Line& other) {
        return other.sharesSameEndpoints(line);
    });
    if(it != constEnd()) {
        result = true;
    }
    return result;
}

Line Line::List::highest() const
{
    Line result;
    if(count() > 0) {
        result = this->at(0);
        for(const Line& line : *this) {
            if(line.isAbove(result)) {
                result = line;
            }
        }
    }
    return result;
}

Line Line::List::lowest() const
{
    Line result;
    if(count() > 0) {
        result = this->at(0);
        for(const Line& line : *this) {
            if(line.isBelow(result)) {
                result = line;
            }
        }
    }
    return result;
}

Line Line::List::shortest() const
{
    Line result;
    for(const Line& line : *this) {
        if(result.isValid() == false || line.length() < result.length()) {
            result = line;
        }
    }
    return result;
}

Line Line::List::longest() const
{
    Line result;
    for(const Line& line : *this) {
        if(result.isValid() == false || line.length() > result.length()) {
            result = line;
        }
    }
    return result;
}

QPointF Line::List::closestPointTo(const QPointF &other)
{
    Line c;
    double d;
    return closestPointTo(other, c, d);
}

QPointF Line::List::closestPointTo(const QPointF& other, Line& closestLine, double &closestDistance)
{
    closestDistance = INFINITY;
    QPointF closestPoint;

    for(const Line& line : *this) {
        double	distance;
        QPointF	p = line.closestPointTo(other, distance);
        if(distance < closestDistance) {
            closestDistance = distance;
            closestPoint = p;
            closestLine = line;
        }
    }
    return closestPoint;
}

double Line::List::minX() const
{
    double result = INFINITY;
    const_iterator it = std::min_element(constBegin(), constEnd(), [](const Line& a, const Line& b)
    {
        return a.minX() < b.minX();
    });
    if(it != constEnd()) {
        Line l = *it;
        result = l.minX();
    }
    return result;
}

double Line::List::maxX() const
{
    double result = 0;
    const_iterator it = std::max_element(constBegin(), constEnd(), [](const Line& a, const Line& b)
                                         {
        return a.maxX() < b.maxX();
    });
    if(it != constEnd()) {
        Line l = *it;
        result = l.maxX();
    }
    return result;
}

double Line::List::minY() const
{
    double result = INFINITY;
    const_iterator it = std::min_element(constBegin(), constEnd(), [](const Line& a, const Line& b)
    {
        return a.minY() < b.minY();
    });
    if(it != constEnd()) {
        Line l = *it;
        result = l.minY();
    }
    return result;
}

double Line::List::maxY() const
{
    double result = 0;
    const_iterator it = std::max_element(constBegin(), constEnd(), [](const Line& a, const Line& b)
    {
        return a.maxY() < b.maxY();
    });

    if(it != constEnd()) {
        Line l = *it;
        result = l.maxY();
    }
    return result;
}

/**
 * @brief Line::List::rectangle
 * @return a rectangle containing all our lines
 */
Rectangle Line::List::boundingRectangle() const
{
    Rectangle result;
    if(count() > 0) {
        Point::List points;
        points.append(Point(minX(), minY()));
        points.append(Point(minX(), maxY()));
        points.append(Point(maxX(), minY()));
        points.append(Point(maxX(), maxY()));
        result = Rectangle::fromPoints(points);
    }
    return result;
}

Line Line::List::lineContainingPoint(const Point &point) const
{
    for(const Line& line : *this) {
        if(line.containsPoint(point)) {
            return line;
        }
    }
    return Line();
}

Line Line::List::lineNearPoint(const Point &point, int margin) const
{
    for(const Line& line : *this) {
        if(line.isPerpendicular()) {
            Rectangle rect = line.makeRectangle(margin / 2);
            if(rect.contains(point)) {
                return line;
            }
        }
        else {
            Polygon polygon = Polygon::fromLine(line, margin);
            if(polygon.contains(point)) {
                return line;
            }
        }
    }
    return Line();
}

double Line::List::totalLength() const
{
    double result = 0;
    for(const Line& line : *this) {
        result += line.length();
    }
    return result;
}

Line::List& Line::List::moveDelta(double dx, double dy)
{
    for(Line& line : *this) {
        line.moveDelta(dx, dy);
    }
    return *this;
}

QList<QLineF> Line::List::toQLineFList() const
{
    QList<QLineF> result;
    for(const Line& line : *this) {
        result.append(line.toQLineF());
    }
    return result;
}

QList<QLine> Line::List::toQLineList() const
{
    QList<QLine> result;
    for(const Line& line : *this) {
        result.append(line.toQLine());
    }
    return result;
}

QString Line::List::toString() const
{
    QString result;
    QTextStream output(&result);
    for(int i = 0;i < count();i++) {
        const Line& line = this->at(i);
        output << QString("(%1)").arg(line.toString());
        if(i < count() - 1) {
            output << ',';
        }
    }
    return result;
}

