#include "rectangle.h"


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

bool Rectangle::isPointOnEdge(const Point &point) const
{
    bool result = false;
    Line::List allLines = edges();
    for(const Line& line : allLines) {
        if(line.containsPoint(point)) {
            result = true;
            break;
        }
    }
    return result;
}

bool Rectangle::containsAnyPoint(const Line &line) const
{
    return contains(line.p1()) || contains(line.p2()) || line.intersects(*this);
}
