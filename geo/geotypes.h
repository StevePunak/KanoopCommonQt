#ifndef GEOTYPES_H
#define GEOTYPES_H

#include <QList>
#include <QRect>
#include <QString>


namespace Geo {

enum CardinalDirection
{
    InvalidCardinalDirection = 0,
    North = 1,
    East = 2,
    South = 3,
    West = 4
};

static inline QString string(const QRect& rect)
{
    return QString("x,y %1,%2  w,h %3,%4").arg(rect.topLeft().x()).arg(rect.topLeft().y()).arg(rect.width()).arg(rect.height());
}

static inline QString string(const QSize& size)
{
    return QString("w,h %1,%2").arg(size.width()).arg(size.height());
}

static inline QString string(const QPoint& point)
{
    return QString("%1,%2").arg(point.x()).arg(point.y());
}

static inline QString string(const QPointF& point)
{
    return QString("%1,%2").arg(point.x()).arg(point.y());
}

}

#endif // GEOTYPES_H
