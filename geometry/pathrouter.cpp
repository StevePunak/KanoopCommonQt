#include "pathrouter.h"
#include "klog.h"

#include "commonexception.h"

using namespace Geo;

PathRouter::PathRouter(const QPoint &origin, const QPoint &destination, const QRectF &canvas, const QList<QRect> &obstacles) :
    _originPoint(origin), _destinationPoint(destination),
    _canvas(canvas),
    _obstacles(obstacles),
    _direction(Geo::NoDirection),
    _routingMargin(DefaultRoutingMargin),
    _debugLevel(KLog::Debug)
{
}

Line::List PathRouter::calcluatePath()
{
    Point a = _originPoint;
    Point b = _destinationPoint;

    logText(KLOG_DEBUG, QString("Calculate path from %1 to %2 with %3 obstacles").arg(a.toString()).arg(b.toString()).arg(_obstacles.count()));

    pointLiesWithinObstacle(_originPoint, _originObstacle);
    pointLiesWithinObstacle(_destinationPoint, _destinationObstacle);

    // cycle on calculation
    while(cycle(a, b) == false) {
        a = _pathLines.last().p2();
    }
    return _pathLines;
}

bool PathRouter::cycle(const Point &a, const Point &b)
{
    bool result = false;

    QList<QRect> obstaclesToIgnore = { _originObstacle, _destinationObstacle };

    Line direct(a, b);
    QRect obstacle;
    Point intersection;
    if(direct.isPerpendicular() && lineCrossesObstacle(direct, obstaclesToIgnore, obstacle, intersection) == false) {
        appendPathLine(direct);
        result = true;      // done!
    }
    else {
        // choose an direction
        _direction = chooseDirection(a, b, _direction);

        Line ray = rayInDirection(a, _direction);
        if(lineCrossesObstacle(ray, obstaclesToIgnore, obstacle, intersection)) {
            // insert a line to midpoint of 'a' and intersection
            Line nextLine(ray.p1(), intersection);
            nextLine = Line(nextLine.p1(), nextLine.midpoint());
            appendPathLine(nextLine);
        }
        else {
            // We have a clear path in this axis.
            // Move along the current direction finding the best path
            Direction desiredDirection = chooseDirection(a, b);     // general direction of target
            if(desiredDirection != _direction) {
                // we have changed directions... seek for a clear path on the new axis
                Line nextLine = seekClearLine(a, _direction, desiredDirection);
                if(nextLine.isValid()) {
                    appendPathLine(nextLine);
                }
                else {
                    logText(KLOG_DEBUG, "No clear line. WHAT NOW?");
                }
            }
            else {
                // we are going the same direction as previous pass... go as far as possible
                // towards our target
                Line checkLine = rayInDirection(a, desiredDirection);
                if(lineCrossesObstacle(checkLine, _originObstacle, obstacle, intersection) == false) {
                    // nothing in the way. go as far as possible towards target
                    checkLine = reduceLineToTarget(checkLine, desiredDirection, b, _routingMargin);
                    appendPathLine(checkLine);
                }
                else if(obstacle == _destinationObstacle) {
                    if(intersection != b) {
                        // we
                    }
                }
                else {
                    logText(KLOG_DEBUG, "Going in same directions WHAT NOW?");
                }
            }
        }
    }
    return result;
}

bool PathRouter::lineCrossesObstacle(const Line &line, QRect &firstObstacle, Point &intersection) const
{
    return lineCrossesObstacle(line, QList<QRect>(), firstObstacle, intersection);
}

bool PathRouter::lineCrossesObstacle(const Line &line, const QRect &ignoreObstacle, QRect &firstObstacle, Point &intersection) const
{
    return lineCrossesObstacle(line, QList<QRect>() << ignoreObstacle, firstObstacle, intersection);
}

bool PathRouter::lineCrossesObstacle(const Line& line, const QList<QRect> &ignoreObstacles, QRect &firstObstacle, Point& intersection) const
{
    bool result = false;
    double minDistance = INFINITY;
    try
    {
        for(const QRect& obstacle : _obstacles) {
            if(ignoreObstacles.contains(obstacle)) {
                continue;
            }

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
                Point ix;
                if(line.intersects(obstacleLine, ix)) {
                    double distance = FlatGeo::distance(line.p1(), ix);
                    if(distance < minDistance) {
                        minDistance = distance;
                        intersection = ix;
                        firstObstacle = obstacle;
                    }
                }
            }
        }

        result = minDistance != INFINITY;
    }
    catch(const CommonException& e)
    {
        logText(KLOG_ERROR, e.message());
        result  = false;
    }
    return result;
}

bool PathRouter::pointLiesWithinObstacle(const Point& point, QRect &result)
{
    result = QRect();
    for(const QRect& obstacle : _obstacles) {
        QRect inflated = obstacle.adjusted(-1, -1, 1, 1);
        if(inflated.contains(point.toPoint())) {
            result = obstacle;
            break;
        }
    }
    return result.isNull() == false;
}

Geo::Direction PathRouter::chooseDirection(const Point &a, const Point &b, Direction exclude) const
{
    Geo::Direction result = Geo::NoDirection;

    double dx = qAbs(a.x() - b.x());
    double dy = qAbs(a.y() - b.y());
    if(b.isRightOf(a)) {
        if(b.isAbove(a)) {
            result = dx < dy ? Up : ToRight;
            if(result == exclude) {
                result = (result == Up) ? ToRight : Up;
            }
        }
        else {
            result = dx < dy ? Down : ToRight;
            if(result == exclude) {
                result = (result == Down) ? ToRight : Down;
            }
        }
    }
    else if(b.isLeftOf(a)) {
        if(b.isAbove(a)) {
            result = dx < dy ? Up : ToLeft;
            if(result == exclude) {
                result = (result == Up) ? ToLeft : Down;
            }
        }
        else {
            result = dx < dy ? Down : ToLeft;
            if(result == exclude) {
                result = (result == Down) ? ToLeft : Down;
            }
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
        logText(KLOG_ERROR, "Invalid direction");
        break;
    }
    result = Line(origin, other);
    return result;
}

Line PathRouter::seekClearLine(const Point &origin, Geo::Direction moveAxis, Direction seekAxis)
{
    Line result;
    Point nextOrigin = origin;

    QList<QRect> obstaclesToIgnore = { _destinationObstacle };

    while(_canvas.contains(nextOrigin)) {
        nextOrigin.move(moveAxis, _routingMargin);
        Line ray = rayInDirection(nextOrigin, seekAxis);
        QRect obstacle;
        Point intersection;

        if(lineCrossesObstacle(ray, obstaclesToIgnore, obstacle, intersection) == false) {
            result = Line(origin, nextOrigin);
            break;
        }
    }
    return result;
}

Line PathRouter::reduceLineToTarget(const Line &line, Geo::Direction direction, const Point &target, double margin)
{
    Line result = line;
    switch(direction) {
    case Up:
        if(line.p2().y() < target.y()) {
            result.setP2(Point(line.p2().x(), target.y()));
        }
        break;
    case Down:
        if(line.p2().y() > target.y()) {
            result.setP2(Point(line.p2().x(), target.y()));
        }
        break;
    case ToLeft:
        if(line.p2().x() < target.x()) {
            result.setP2(Point(target.x(), line.p2().y()));
        }
        break;
    case ToRight:
        if(line.p2().x() > target.x()) {
            result.setP2(Point(target.x(), line.p2().y()));
        }
        break;
    default:
        break;
    }

    if(margin != 0 && result.length() > margin) {
        result.shorten(margin);
        result.round();
    }

    return result;
}

void PathRouter::appendPathLine(const Line &line)
{
    logText(KLOG_DEBUG, QString("Appending path line %1").arg(line.toString()));
    _pathLines.append(line);
}

void PathRouter::logText(const char *file, int line, int level, const QString &text) const
{
    if(level <= _debugLevel) {
        KLog::sysLogText(file, line, (KLog::LogLevel)level, text);
    }
}
