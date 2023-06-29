#include "angle.h"
#include "circle.h"
#include "flatgeo.h"
#include "line.h"
#include <QtMath>

double FlatGeo::vectorAngle(const Line &l1, const Line &l2)
{
    double		a = l1.p1().x() - l1.p1().x();
    double		b = l1.p2().y() - l1.p1().y();
    double		c = l2.p2().x() - l2.p1().x();
    double		d = l2.p2().y() - l2.p1().y();

    double 		z =((a*c) + (b*d)) / (qSqrt((a*a) + (b*b)) * qSqrt((c*c) + (d*d)));
    double		theta = qAcos(z);
    double		result = degrees(theta);

    return result;
}

Angle FlatGeo::angle(const Line &l1, const Line &l2)
{
    double result = qAtan2(l1.p2().y() - l1.p1().y(), l1.p2().x() - l1.p1().x()) - qAtan2(l2.p2().y() - l2.p1().y(), l2.p2().x() - l2.p1().x());
    result = degrees(result);
    return Angle(qFabs(result));
}

Angle FlatGeo::angle(const QPointF &p1, const QPointF &vertex, const QPointF &p2)
{
    return anglev2(Line(vertex, p1), Line(vertex, p2));
}

double FlatGeo::distance(const QPointF &p1, const QPointF &p2)
{
    return Line(p1, p2).length();
}

double FlatGeo::degrees(double angle)
{
    return qRadiansToDegrees(angle);
}

double FlatGeo::radians(double angle)
{
    return qDegreesToRadians(angle);
}

double FlatGeo::angularDifference(const Line &l1, const Line &l2)
{
    return qFabs((l1.bearing() + 360) - (l2.bearing() + 360));
}

QPointF FlatGeo::getPoint(const QPointF &from, double bearing, double distance)
{
    double radBearing = radians(bearing);
    return QPointF(from.x() + qSin(radBearing) * distance, from.y() - qCos(radBearing) * distance);
}

QPointF FlatGeo::intersection(const Line &l1, const Line &l2)
{
    QPointF result;

    double A1, B1, C1;
    getLineABC(l1, A1, B1, C1);

    double A2, B2, C2;
    getLineABC(l2, A2, B2, C2);

    double det = A1*B2 - A2*B1;
    if(det != 0)
    {
        double resultX = ( (B2*C1 - B1*C2) / det);
        double resultY = ( (A1*C2 - A2*C1) / det);

        if(	resultX >= qMin(l1.p1().x(), l1.p2().x()) && resultX <= qMax(l1.p1().x(), l1.p2().x()) &&
            resultX >= qMin(l2.p1().x(), l2.p2().x()) && resultX <= qMax(l2.p1().x(), l2.p2().x()) &&
            resultY >= qMin(l1.p1().y(), l1.p2().y()) && resultY <= qMax(l1.p1().y(), l1.p2().y()) &&
            resultY >= qMin(l2.p1().y(), l2.p2().y()) && resultY <= qMax(l2.p1().y(), l2.p2().y()) )
        {
            result = QPointF(resultX, resultY);
        }
    }
    // else, its parallel
    return result;
}

bool FlatGeo::isInCircle(const QPointF &point, const Circle &circle)
{
    Line l(circle.center(), point);
    return (l.length() <= circle.radius());
}

void FlatGeo::getLineABC(const Line &line, double &a, double &b, double &c)
{
    a = line.p2().y() - line.p1().y();
    b = line.p1().x() - line.p2().x();
    c = a * line.p1().x() + b * line.p1().y();
}

QPointF FlatGeo::move(const QPointF &point, double bearing, double distance)
{
    QPointF result = FlatGeo::getPoint(point, bearing, distance);
    return result;
}

QPointF FlatGeo::rotate(const QPointF &point, const QPointF &centroid, double angle)
{
    double x = point.x();
    double y = point.y();
    x += (0 - centroid.x());
    y += (0 - centroid.y());

    QPointF np;
    np.setX(((x * qCos(angle * (M_PI / 180))) - (y * qSin(angle * (M_PI / 180)))) + centroid.x());
    np.setY((qSin(angle * (M_PI / 180)) * x + qCos(angle * (M_PI / 180)) * y) + centroid.y());

    return QPointF(x, y);
}

bool FlatGeo::arePointsEqual(const QPointF &p1, const QPointF &p2, int precision)
{
    bool result = false;
    if(precision == 0) {
        result = p1.x() == p2.x() && p1.y() == p2.y();
    }
    else {
        QString x1 = roundedString(p1.x(), precision);
        QString y1 = roundedString(p1.y(), precision);
        QString x2 = roundedString(p2.x(), precision);
        QString y2 = roundedString(p2.y(), precision);
        result = x1 == x2 && y1 == y2;
    }
    return result;
}

QString FlatGeo::makePointString(const QPoint &p)
{
    return QString("%1, %2").arg(p.x()).arg(p.y());
}

QString FlatGeo::makePointString(const QPointF &p)
{
    return QString("%1, %2").arg(p.x()).arg(p.y());
}

Angle FlatGeo::anglev2(const Line &l1, const Line &l2)
{
    double ret = qAtan2(l1.p2().y() - l1.p1().y(), l1.p2().x() - l1.p1().x()) - qAtan2(l2.p2().y() - l2.p1().y(), l2.p2().x() - l2.p1().x());
    ret = degrees(ret);
    if(ret < 0) {
        ret += 360;
    }
    return Angle(qFabs(ret));
}

QString FlatGeo::roundedString(double value, int precision)
{
    return QString("%1").arg(value, precision, 'f');
}

QList<Geo::Direction> Geo::allDirections()
{
    static const QList<Direction> directions =
        {
            Up, Down, ToLeft, ToRight
        };
    return directions;
}

Geo::Side Geo::directionToSide(Direction direction)
{
    return (Side)direction;
}

Geo::Direction Geo::sideToDirection(Side side)
{
    return (Direction)side;
}
