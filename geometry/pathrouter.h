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
    PathRouter() :
        _direction(Geo::NoDirection),
        _routingMargin(DefaultRoutingMargin),
        _debugLevel(0) {}
    PathRouter(const QPoint& origin, const QPoint& destination, const QRectF &canvas, const QList<QRect>& obstacles = QList<QRect>());

    Line::List calcluatePath();

    void appendObstacle(const QRect& value) { _obstacles.append(value); }
    void setObstacles(const QList<QRect>& value) { _obstacles = value; }

    Point originPoint() const { return _originPoint; }
    void setOriginPoint(const Point& value) { _originPoint = value ; }

    Point destinationPoint() const { return _destinationPoint; }
    void setDestinationPoint(const Point& value) { _destinationPoint = value; }

    int routingMargin() const { return _routingMargin; }
    void setRoutingMargin(int value) { _routingMargin = value; }

private:
    bool cycle(const Point& a, const Point& b);

    bool lineCrossesObstacle(const Line &line, QRect& firstObstacle, Point &intersection) const;
    bool lineCrossesObstacle(const Line &line, const QRect &ignoreObstacle, QRect& firstObstacle, Point &intersection) const;
    bool lineCrossesObstacle(const Line &line, const QList<QRect> &ignoreObstacles, QRect& firstObstacle, Point &intersection) const;
    bool pointLiesWithinObstacle(const Point &point, QRect& result);
    Geo::Direction chooseDirection(const Point& a, const Point& b, Geo::Direction exclude = Geo::NoDirection) const;
    Line rayInDirection(const Point& origin, Geo::Direction direction);
    Line seekClearLine(const Point& origin, Geo::Direction moveAxis, Geo::Direction seekAxis);
    Line reduceLineToTarget(const Line& line, Geo::Direction direction, const Point& target, double margin);

    void appendPathLine(const Line& line);

    void logText(const char* file, int line, int level, const QString& text) const;

    Point _originPoint;
    Point _destinationPoint;
    QRectF _canvas;
    QList<QRect> _obstacles;
    Geo::Direction _direction;
    int _routingMargin;
    int _debugLevel;

    Line::List _pathLines;
    QRect _originObstacle;
    QRect _destinationObstacle;

    static const int DefaultRoutingMargin = 10;
};

#endif // PATHROUTER_H
