#include "Kanoop/geometry/rectangle.h"

#include <Kanoop/geometry/size.h>

#include <QLocale>
#include <QRegularExpression>

using namespace Geo;

Rectangle Rectangle::fromPoints(const Point::List &points)
{
    Rectangle result;
    Point topLeft = points.topLeft();
    Point bottomLeft = points.bottomLeft();
    Point topRight = points.topRight();
    Point bottomRight = points.bottomRight();
    if( topLeft.x() == bottomLeft.x() &&
        topRight.x() == bottomRight.x() &&
        topLeft.y() == topRight.y() &&
        bottomLeft.y() == bottomRight.y()) {
        result = Rectangle(topLeft.x(), topLeft.y(), topRight.x() - topLeft.x(), bottomLeft.y() - topLeft.y());
    }
    return result;
}

Rectangle Rectangle::fromPoints(const Point &p1, const Point &p2)
{
    int minX = qMin(p1.x(), p2.x());
    int minY = qMin(p1.y(), p2.y());
    int maxX = qMax(p1.x(), p2.x());
    int maxY = qMax(p1.y(), p2.y());
    Rectangle rect(minX, minY, maxX - minX, maxY - minY);
    return rect;
}

Rectangle Rectangle::fromCenterLine(const Line &centerLine, double expand)
{
    Rectangle result;
    Line l1 = centerLine;
    Line l2 = centerLine;
    double d = expand / 2;
    if(centerLine.isHorizontal()) {
        l1.move(Up, d);
        l2.move(Down, d);
    }
    else {
        l1.move(ToLeft, d);
        l2.move(ToRight, d);
    }
    double minX = qMin(l1.minX(), l2.minX());
    double maxX = qMax(l1.maxX(), l2.maxX());
    double minY = qMin(l1.minY(), l2.minY());
    double maxY = qMax(l1.maxY(), l2.maxY());
    double w = maxX - minX;
    double h = maxY - minY;
    result = Rectangle(minX, minY, w, h);
    return result;
}

Rectangle Rectangle::fromCenterPoint(const Point &centerPoint, double expand)
{
    double x = centerPoint.x() - expand;
    double y = centerPoint.y() - expand;
    Rectangle result(x, y, expand * 2, expand * 2);
    return result;
}

Rectangle Rectangle::fromString(const QString& value)
{
    Rectangle result;

    static const QRegularExpression regex(QStringLiteral("[+-]"));

    int index = value.indexOf(regex);
    if(index > 0) {
        double w = 0, h = 0, x = 0, y = 0;
        QString sizePart = value.mid(0, index);
        QString posPart = value.mid(index);
        QStringList sizeParts = sizePart.split('x');
        if(sizeParts.count() == 2) {
            w = sizeParts[0].toDouble();
            h = sizeParts[1].toDouble();
        }

        if(posPart.length() > 1) {
            int split = posPart.indexOf(regex, 1);
            if(split > 0) {
                QString xs = posPart.mid(0, split);
                QString ys = posPart.mid(split);
                x = xs.toDouble();
                y = ys.toDouble();

                result = Rectangle(x, y, w, h);
            }
        }
    }
    return result;
}

Point Rectangle::closestCorner(const Point &origin) const
{
    Line::List lines;
    lines.append(Line(origin, topLeft()));
    lines.append(Line(origin, topRight()));
    lines.append(Line(origin, bottomLeft()));
    lines.append(Line(origin, bottomRight()));
    Line shortest = lines.shortest();
    return shortest.p2();
}

Point::List Rectangle::corners() const
{
    Point::List result;
    result.append(topLeft());
    result.append(topRight());
    result.append(bottomLeft());
    result.append(bottomRight());
    return result;
}

Line::List Rectangle::edges() const
{
    Line::List result;
    result.append(verticalLines());
    result.append(horizontalLines());
    return result;
}

QMap<Geo::Side, Line> Rectangle::edgeMap() const
{
    QMap<Geo::Side, Line> result;
    result.insert(Top, topEdge());
    result.insert(Bottom, bottomEdge());
    result.insert(Left, leftEdge());
    result.insert(Right, rightEdge());
    return result;
}

Line::List Rectangle::verticalLines() const
{
    Line::List result;
    result.append(Line(topLeft(), bottomLeft()));
    result.append(Line(topRight(), bottomRight()));
    return result;
}

Line::List Rectangle::horizontalLines() const
{
    Line::List result;
    result.append(Line(topLeft(), topRight()));
    result.append(Line(bottomLeft(), bottomRight()));
    return result;
}

Line Rectangle::topEdge() const
{
    return Line(topLeft(), topRight());
}

Line Rectangle::bottomEdge() const
{
    return Line(bottomLeft(), bottomRight());
}

Line Rectangle::leftEdge() const
{
    return Line(topLeft(), bottomLeft());
}

Line Rectangle::rightEdge() const
{
    return Line(topRight(), bottomRight());
}

Line Rectangle::closestEdge(const Point &point)
{
    Line result;
    double closestDistance = INFINITY;
    for(const Line& edge : edges()) {
        double distance;
        edge.closestPointTo(point, distance);
        if(distance < closestDistance) {
            closestDistance = distance;
            result = edge;
        }
    }
    return result;
}

bool Rectangle::isPointOnEdge(const Point &point, Line &foundEdge) const
{
    bool result = false;
    Line::List allLines = edges();
    for(const Line& line : allLines) {
        if(line.containsPoint(point)) {
            result = true;
            foundEdge = line;
            break;
        }
    }
    return result;
}

bool Rectangle::isPointOnEdge(const Point &point) const
{
    Line edge;
    return isPointOnEdge(point, edge);
}

bool Rectangle::containsAnyPoint(const Line &line) const
{
    return contains(line.p1()) || contains(line.p2()) || line.intersects(*this);
}

Point Rectangle::centerPoint() const
{
    return Line(topLeft(), bottomRight()).midpoint();
}

Geo::Side Rectangle::closestSideToPoint(const Point& point) const
{
    Side result = NoSide;
    double closestDistance = INFINITY;
    QMap<Geo::Side, Line> edges = edgeMap();
    Point closestPoint;
    for(QMap<Geo::Side, Line>::const_iterator it = edges.constBegin();it != edges.constEnd();it++) {
        Line edge = it.value();
        double distance;
        Point p = edge.closestPointTo(point, distance);
        if(distance < closestDistance) {
            closestPoint = p;
            closestDistance = distance;
            result = it.key();
        }
    }
    return result;
}

Rectangle& Rectangle::scale(double scale)
{
    Size size = Rectangle::size();
    size.setWidth(width() * scale);
    size.setHeight(height() * scale);
    setSize(size);
    return *this;
}

Rectangle Rectangle::scaled(double scale) const
{
    Rectangle rect = *this;
    rect.scale(scale);
    return rect;
}

Rectangle& Rectangle::grow(double amount)
{
    QRectF::adjust(-amount, -amount, amount, amount);
    return *this;
}

Rectangle Rectangle::grown(double amount) const
{
    Rectangle result = *this;
    result.grow(amount);
    return result;
}

QString Rectangle::toString() const
{
    QString posString = QString("%1%2%3%4")
                            .arg(x() >= 0 ? '+' : '-').arg(x(), 0, 'f', QLocale::FloatingPointShortest)
                            .arg(y() >= 0 ? '+' : '-').arg(y(), 1, 'f', QLocale::FloatingPointShortest);
    return QString("%1x%2%3")
        .arg(width(), 0, 'f', QLocale::FloatingPointShortest)
        .arg(height(), 1, 'f', QLocale::FloatingPointShortest)
        .arg(posString);
}
