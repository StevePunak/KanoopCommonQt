#include "Kanoop/geometry/point.h"
#include "Kanoop/geometry/flatgeo.h"

using namespace Geo;

Point &Point::move(Direction direction, double amount)
{
    switch(direction) {
    case Up:
        setY(y() - amount);
        break;
    case Down:
        setY(y() + amount);
        break;
    case ToLeft:
        setX(x() - amount);
        break;
    case ToRight:
        setX(x() + amount);
        break;
    default:
        break;
    }
    return *this;
}

Point &Point::move(double bearing, double distance)
{
    Point np = FlatGeo::getPoint(*this, bearing, distance);
    this->rx() = np.x();
    this->ry() = np.y();
    return *this;
}

Point &Point::moveDelta(double dx, double dy)
{
    rx() += dx;
    ry() += dy;
    return *this;
}

Point &Point::round()
{
    setX(qRound(x()));
    setY(qRound(y()));
    return *this;
}

Point &Point::offset(double x, double y)
{
    rx() += x;
    ry() += y;
    return *this;
}

Point Point::delta(const Point &other)
{
    Point result(x() - other.x(), y() - other.y());
    return result;
}

Point Point::fromString(const QString &value)
{
    Point result;
    QStringList parts = value.split(',', Qt::SkipEmptyParts);
    if(parts.count() == 2) {
        double x = parts.at(0).trimmed().toDouble();
        double y = parts.at(1).trimmed().toDouble();
        result = Point(x, y);
    }
    return result;
}

Point Point::List::topLeft() const
{
    Point result;
    for(int i = 0;i < count();i++) {
        Point p = this->at(i);
        if(i == 0 || (p.x() <= result.x() && p.y() <= result.y())) {
            result = p;
        }
    }
    return result;
}

Point Point::List::topRight() const
{
    Point result;
    for(int i = 0;i < count();i++) {
        Point p = this->at(i);
        if(i == 0 || (p.x() >= result.x() && p.y() <= result.y())) {
            result = p;
        }
    }
    return result;
}

Point Point::List::bottomLeft() const
{
    Point result;
    for(int i = 0;i < count();i++) {
        Point p = this->at(i);
        if(i == 0 || (p.x() <= result.x() && p.y() >= result.y())) {
            result = p;
        }
    }
    return result;
}

Point Point::List::bottomRight() const
{
    Point result;
    for(int i = 0;i < count();i++) {
        Point p = this->at(i);
        if(i == 0 || (p.x() >= result.x() && p.y() >= result.y())) {
            result = p;
        }
    }
    return result;
}
