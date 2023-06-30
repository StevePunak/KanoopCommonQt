#ifndef PATHROUTER_H
#define PATHROUTER_H

#include "line.h"

#include <QList>
#include <QRect>
#include <QString>

#include "point.h"

class PathRouter
{
public:
    PathRouter() : _debugLevel(0) {}
    PathRouter(const QPoint& origin, const QPoint& destination, const QList<QRect>& obstacles = QList<QRect>());

    Line::List calcluatePath();

    void appendObstacle(const QRect& value) { _obstacles.append(value); }
    void setObstacles(const QList<QRect>& value) { _obstacles = value; }

private:
    void recursiveCalculatePath(const Point& a, const Point& b);

    bool lineCrossesObstacle(const Line &line, QRect& firstObstacle) const;
    Geo::Direction chooseDirection(const Point& a, const Point& b) const;

    void debugLog(const char* file, int line, int level, const QString& text);

    Point _originPoint;
    Point _destinationPoint;
    QList<QRect> _obstacles;

    int _debugLevel;
};

#endif // PATHROUTER_H
