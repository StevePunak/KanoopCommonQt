#include "pathrouter.h"
#include "klog.h"

#include "commonexception.h"

using namespace Geo;

PathRouter::PathRouter(const QPoint &origin, const QPoint &destination, const QRect& canvas, const QList<QRect> &obstacles) :
    _originPoint(origin), _destinationPoint(destination),
    _canvas(canvas),
    _obstacles(obstacles),
    _routingMargin(DefaultRoutingMargin),
    _debugLevel(KLog::Debug)
{
}

Line::List PathRouter::calcluatePath()
{
    Line::List result;
    recursiveCalculatePath(_originPoint, _destinationPoint, result);
    return result;
}

bool PathRouter::recursiveCalculatePath(const Point &a, const Point &b, Line::List& pathLines)
{
    bool result = false;
    //
    Line direct(a, b);
    QRect obstacle;
    Point intersection;
    if(direct.isPerpendicular() && lineCrossesObstacle(direct, obstacle, intersection) == false) {
        pathLines.append(direct);
        result = true;      // done!
    }
    else {
        Direction direction = chooseDirection(a, b);
        Line line = rayInDirection(a, direction);
        if(lineCrossesObstacle(line, obstacle, intersection)) {
            // insert a line to midpoint of 'a' and intersection
            line = Line(line.p1(), intersection);
            line = Line(line.p1(), line.midpoint());
            pathLines.append(line);
        }
        else {
            // we have a clear path to 'b' in this axis.
            // Shorten it to distance - margin

        }
    }


    return result;
}

bool PathRouter::lineCrossesObstacle(const Line& line, QRect &firstObstacle, Point& intersection) const
{
    bool result = false;
    double minDistance = INFINITY;
    try
    {
        for(const QRect& obstacle : _obstacles) {
            Line::List obstacleLines;
            if(line.isHorizontal()) {
                obstacleLines = Line::verticalLines(obstacle);
            }
            else if(line.isVertical()) {
                obstacleLines = Line::horizontalLines(obstacle);
            }
            else {
                throw CommonException("Found a non perpendicular line (bug)");
            }

            for(const Line& obstacleLine : obstacleLines) {
                if(line.intersects(obstacleLine, intersection)) {
                    double distance = FlatGeo::distance(line.p1(), intersection);
                    if(distance < minDistance) {
                        minDistance = distance;
                        firstObstacle = obstacle;
                    }
                }
            }
        }

        result = minDistance != INFINITY;
    }
    catch(const CommonException& e)
    {
        debugLog(KLOG_ERROR, e.message());
        result  = false;
    }
    return result;
}

Geo::Direction PathRouter::chooseDirection(const Point &a, const Point &b) const
{
    Geo::Direction result = Geo::NoDirection;

    double dx = qAbs(a.x() - b.x());
    double dy = qAbs(a.y() - b.y());
    if(b.isRightOf(a)) {
        if(b.isAbove(a)) {
            result = dx > dy ? Up : ToRight;
        }
        else {
            result = dx > dy ? Down : ToRight;
        }
    }
    else if(b.isLeftOf(a)) {
        if(b.isAbove(a)) {
            result = dx > dy ? Up : ToLeft;
        }
        else {
            result = dx > dy ? Down : ToLeft;
        }
    }
    else {
        if(b.isAbove(a)) {
            result = Up;
        }
        else {
            result = Down;
        }
    }
    return result;
}

Line PathRouter::rayInDirection(const Point &origin, Geo::Direction direction)
{
    Line result;
    Point other;
    switch(direction) {
    case Up:
        other = Point(origin.x(), 0);
        break;
    case Down:
        other = Point(origin.x(), _canvas.bottom());
        break;
    case ToLeft:
        other = Point(0, origin.y());
        break;
    case ToRight:
        other = Point(_canvas.right(), origin.y());
        break;
    default:
        debugLog(KLOG_ERROR, "Invalid direction");
        break;
    }
    result = Line(origin, other);
    return result;
}

void PathRouter::debugLog(const char *file, int line, int level, const QString &text) const
{
    if(level <= _debugLevel) {
        KLog::sysLogText(file, line, (KLog::LogLevel)level, text);
    }
}
