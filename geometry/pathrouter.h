#ifndef PATHROUTER_H
#define PATHROUTER_H

#include "line.h"

#include <QList>
#include <QRect>
#include <QString>

#include "point.h"
#include "rectangle.h"

class PathRouter
{
public:
    PathRouter() :
        _direction(Geo::NoDirection),
        _forceDirection(Geo::NoDirection),
        _complete(false),
        _routingMargin(DefaultRoutingMargin),
        _debugLevel(0) {}
    PathRouter(const Point& origin, const Point& destination, const QRectF &canvas, const QList<Rectangle> &obstacles = QList<Rectangle>());

    Line::List calcluatePath();

    void appendObstacle(const Rectangle& value) { _obstacles.append(value); }
    void setObstacles(const QList<Rectangle>& value) { _obstacles = value; }

    Point originPoint() const { return _originPoint; }
    void setOriginPoint(const Point& value) { _originPoint = value ; }

    Point destinationPoint() const { return _destinationPoint; }
    void setDestinationPoint(const Point& value) { _destinationPoint = value; }

    int routingMargin() const { return _routingMargin; }
    void setRoutingMargin(int value) { _routingMargin = value; }

private:
    bool cycle(const Point& a, const Point& b);
    bool pass1(const Point& a, const Point& b);
    bool pass2(const Point& a, const Point& b);
    bool pass3(const Point& a, const Point& b);

    bool lineCrossesObstacle(const Line &line, Rectangle &firstObstacle, Point &intersection) const;
    bool lineCrossesObstacle(const Line &line, const Rectangle &ignoreObstacle, Rectangle &firstObstacle, Point &intersection) const;
    bool lineCrossesObstacle(const Line &line, const QList<Rectangle> &ignoreObstacles, Rectangle &firstObstacle, Point &intersection) const;
    bool pointLiesWithinObstacle(const Point &point, Rectangle &result);

    static QList<Geo::Direction> chooseDirections(const Point& a, const Point& b);
    static Geo::Direction chooseDirection(const Point& a, const Point& b, Geo::Direction exclude = Geo::NoDirection);
    static Geo::Direction chooseDirection(const Point& a, const Point& b, const QList<Geo::Direction> exclude);
    static Geo::Direction nonExcludedDirection(QList<Geo::Direction> choices, QList<Geo::Direction> exclude);
    static Geo::Direction directionOfLine(const Line& line);

    Line rayInDirection(const Point& origin, Geo::Direction direction);
    Line seekClearLine(const Point& origin, Geo::Direction moveAxis, Geo::Direction seekAxis, double distanceLimit = 0);
    Line reduceLineToTarget(const Line& line, Geo::Direction direction, const Point& target, double margin);
    double distanceInDirection(const Point& origin, Geo::Direction direction, const Point& target);
    Line findPathOffObstacle(const Point& origin, const Rectangle &obstacle, Geo::Direction& direction);

    void appendPathLine(const Line& line);

    void logText(const char* file, int line, int level, const QString& text) const;

    Point _originPoint;
    Point _destinationPoint;
    Rectangle _canvas;
    QList<Rectangle> _obstacles;
    Geo::Direction _direction;
    Geo::Direction _forceDirection;
    bool _complete;
    int _routingMargin;
    int _debugLevel;

    Line::List _pathLines;
    Rectangle _originObstacle;
    Rectangle _destinationObstacle;
    Rectangle _currentObstacle;


    static const int DefaultRoutingMargin = 10;
};

#endif // PATHROUTER_H
