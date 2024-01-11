#ifndef PATHROUTER_H
#define PATHROUTER_H

#include "line.h"

#include <QList>
#include <QRect>
#include <QString>

#include "point.h"
#include "rectangle.h"
#include "Kanoop/kanoopcommon.h"

class KANOOP_EXPORT PathRouter
{
public:
    PathRouter() :
        _direction(Geo::NoDirection),
        _forceDirection(Geo::NoDirection),
        _initialDirectionForced(false),
        _firstSegmentLength(0),
        _complete(false),
        _routingMargin(DefaultRoutingMargin),
        _consolidateEmptyRectangles(true),
        _routeAroundMargins(true),
        _verticalConstraint(-1),
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

    bool consolidateEmptyRectangles() const { return _consolidateEmptyRectangles; }
    void setConsolidateEmptyRectangles(bool value) { _consolidateEmptyRectangles = value; }

    bool routeAroundMargins() const { return _routeAroundMargins; }
    void setRouteAroundMargins(bool value) { _routeAroundMargins = value; }

    void setInitialDirection(Geo::Direction direction) { _forceDirection = direction; }
    void setFirstSegmentLength(double value) { _firstSegmentLength = value; }

    int verticalConstraint() const { return _verticalConstraint; }
    void setVerticalConstraint(int value) { _verticalConstraint = value; }

    int debugLevel() const { return _debugLevel; }
    void setDebugLevel(int value) { _debugLevel = value; }

    static void mergeAdjacentLines(Line::List& lines);

private:
    bool pass1();
    bool pass2();
    bool pass3();
    bool cycleOnPass1(const Point& a, const Point& b);
    bool cycleFindDirectShot(const Point& a, const Point& b);
    bool cycleChooseDirection(const Point& a, const Point& b);
    bool cycleSimpleLShot(const Point &a, const Point &b);
    bool cycleFindWayOffObstacle(const Point& a, const Point& b);
    bool cycleFindNextPoint(const Point& a, const Point& b);

    bool rectangleCrossesObstacle(const Rectangle &rectangle, const QList<Rectangle> &ignoreObstacles = QList<Rectangle>()) const;
    bool linesCrossObstacle(const Line::List &lines) const;
    bool lineCrossesObstacle(const Line &line) const;
    bool lineCrossesObstacle(const Line &line, Rectangle &firstObstacle, Point &intersection) const;
    bool lineCrossesObstacle(const Line &line, const Rectangle &ignoreObstacle, Rectangle &firstObstacle, Point &intersection) const;
    bool lineCrossesObstacle(const Line &line, const QList<Rectangle> &ignoreObstacles, Rectangle &firstObstacle, Point &intersection) const;
    bool pointLiesWithinObstacle(const Point &point, Rectangle &result);
    bool pointLiesOnObstacleEdge(const Point &point, Rectangle &result);
    bool pointLiesOnObstacleEdge(const Point &point, Rectangle &result, Line& foundEdge);
    bool lineLiesOnObstacleEdge(const Line &line, Rectangle &foundObstacle, Line& foundEdge);
    bool lineLiesNearObstacleEdge(const Line &line, double margin, Rectangle &foundObstacle, Line& foundEdge);
    void replaceLines(int index, int count, const Line::List &newLines);
    Geo::Direction directionOffObstacle(const Line& line, const Rectangle& obstacle);

    static QList<Geo::Direction> chooseDirections(const Point& a, const Point& b);
    static Geo::Direction chooseDirection(const Point& a, const Point& b, Geo::Direction exclude = Geo::NoDirection);
    static Geo::Direction chooseDirection(const Point& a, const Point& b, const QList<Geo::Direction> exclude);
    static Geo::Direction nonExcludedDirection(QList<Geo::Direction> choices, QList<Geo::Direction> exclude);
    static Geo::Direction directionOfLine(const Line& line);

    Rectangle makeRectangleFromThreeLines(const Line::List &lines, Line &newLine) const;

    Line rayInDirection(const Point& origin, Geo::Direction direction) const;
    Line seekClearLine(const Point& origin, Geo::Direction moveAxis, Geo::Direction seekAxis, double distanceLimit = 0);
    Line reduceLineToTarget(const Line& line, Geo::Direction direction, const Point& target, double margin);
    double distanceInDirection(const Point& origin, Geo::Direction direction, const Point& target) const;
    Line findClosestCornerPathOffObstacle(const Point& origin, const Rectangle &obstacle, Geo::Direction& resultDirection) const;
    Line findPathOffObstacleInDirections(const Point& origin, const Rectangle &obstacle, const QList<Geo::Direction>& directions) const;
    bool pointLiesOnAppropriateObtstacleCorner(const Point& origin, const Rectangle &obstacle, const QList<Geo::Direction>& directions) const;
    Geo::Direction lastLineDirection() const;
    void removeZeroLengthLines(Line::List& lines) const;
    void mergeTinySegments(Line::List& lines) const;
    bool lineCrossesVerticalConstraint(const Line& line);
    static int countAdjacentLines(const Line::List& lines);

private:
    void appendPathLine(const Line& line);
    void dumpPathLines(const QString &label) const;

    void logText(const char* file, int line, int level, const QString& text) const;

    Point _originPoint;
    Point _destinationPoint;
    Rectangle _canvas;
    QList<Rectangle> _obstacles;
    Geo::Direction _direction;
    Geo::Direction _forceDirection;
    bool _initialDirectionForced;
    double _firstSegmentLength;
    bool _complete;
    int _routingMargin;
    bool _consolidateEmptyRectangles;
    bool _routeAroundMargins;
    double _verticalConstraint;
    int _debugLevel;

    Line::List _pathLines;
    Rectangle _originObstacle;
    Rectangle _destinationObstacle;
    Rectangle _currentObstacle;


    static const int DefaultRoutingMargin = 10;
};

#endif // PATHROUTER_H
