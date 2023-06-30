#include "angle.h"
#include "circle.h"
#include "flatgeo.h"
#include "line.h"

#include <QLine>
#include <QLineF>
#include <QtMath>
#include <float.h>

Line::Line(const QPointF &origin, double bearing, double distance) :
    _p1(origin), _p2(FlatGeo::getPoint(origin, bearing, distance)) {}

QPointF Line::midpoint() const
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

QPointF Line::closestPointTo(const QPointF &point) const
{
    double distance;
    return closestPointTo(point, distance);
}

QPointF Line::closestPointTo(const QPointF &point, double &distance) const
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

QPointF Line::furthestPointFrom(const QPointF &point) const
{
    double distance;
    QPointF closest = closestPointTo(point, distance);
    QPointF furthest = closest == _p1 ? _p2 : _p1;
    return furthest;
}

QPointF Line::topMostPoint() const
{
    return FlatGeo::isPointAbove(_p1, _p2) ? _p1 : _p2;
}

QPointF Line::bottomMostPoint() const
{
    return FlatGeo::isPointBelow(_p1, _p2) ? _p1 : _p2;
}

QPointF Line::leftMostPoint() const
{
    return FlatGeo::isPointLeftOf(_p1, _p2) ? _p1 : _p2;
}

QPointF Line::rightMostPoint() const
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

bool Line::intersects(const Line &other, QPointF &intersection) const
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

QPointF Line::intersection(const Line &other) const
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

void Line::move(double bearing, double distance)
{
    _p1 = FlatGeo::move(_p1, bearing, distance);
    _p2 = FlatGeo::move(_p2, bearing, distance);
}

void Line::rotate(const QPointF &centroid, double angle)
{
    _p1 = FlatGeo::rotate(_p1, centroid, angle);
    _p2 = FlatGeo::rotate(_p2, centroid, angle);
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
    return QString("%1 <==> %2").arg(FlatGeo::makePointString(_p1)).arg(FlatGeo::makePointString(_p2));
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

