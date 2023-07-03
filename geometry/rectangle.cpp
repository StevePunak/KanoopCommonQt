#include "rectangle.h"


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

Line::List Rectangle::lines() const
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
