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
    PathRouter() : _routingMargin(DefaultRoutingMargin), _debugLevel(0) {}
    PathRouter(const QPoint& origin, const QPoint& destination, const QRect &canvas, const QList<QRect>& obstacles = QList<QRect>());

    Line::List calcluatePath();

    void appendObstacle(const QRect& value) { _obstacles.append(value); }
    void setObstacles(const QList<QRect>& value) { _obstacles = value; }

    int routingMargin() const { return _routingMargin; }
    void setRoutingMargin(int value) { _routingMargin = value; }

private:
    bool recursiveCalculatePath(const Point& a, const Point& b, Line::List &pathLines);

    bool lineCrossesObstacle(const Line &line) const;
    bool lineCrossesObstacle(const Line &line, QRect& firstObstacle, Point &intersection) const;
    Geo::Direction chooseDirection(const Point& a, const Point& b) const;
    Line rayInDirection(const Point& origin, Geo::Direction direction);

    void debugLog(const char* file, int line, int level, const QString& text) const;

    Point _originPoint;
    Point _destinationPoint;
    QRect _canvas;
    QList<QRect> _obstacles;

    int _routingMargin;

    int _debugLevel;

    static const int DefaultRoutingMargin = 10;
};

#endif // PATHROUTER_H
