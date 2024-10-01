#include "Kanoop/geometry/size.h"
#include <QStringList>
#include <Kanoop/geometry/rectangle.h>

Rectangle Size::toRectangle() const
{
    return Rectangle(0, 0, width(), height());
}

void Size::grow(double amount)
{
    setWidth(width() + amount);
    setHeight(height() + amount);
}

void Size::shrink(double amount)
{
    setWidth(width() - amount);
    setHeight(height() - amount);
}

Size Size::fromString(const QString& value)
{
    Size result;
    QStringList parts = value.split(',', Qt::SkipEmptyParts);
    if(parts.count() == 2) {
        double w = parts.at(0).trimmed().toDouble();
        double h = parts.at(1).trimmed().toDouble();
        result = Size(w, h);
    }
    return result;
}
