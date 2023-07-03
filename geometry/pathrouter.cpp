/**
 * pathrouter.cpp
 *
 * Works in multiple passes:
 *
 * Pass 1 - Find a direct shot from current 'a' to 'b'.
 *   This pass will find a home run if it exists.
 *   It is the pass which will finally yield a result;
 *
 * Pass 2 -
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
#include "pathrouter.h"
#include "klog.h"

#include "commonexception.h"

using namespace Geo;

PathRouter::PathRouter(const Point &origin, const Point &destination, const QRectF &canvas, const QList<Rectangle> &obstacles) :
    _originPoint(origin), _destinationPoint(destination),
    _canvas(canvas),
    _obstacles(obstacles),
    _direction(Geo::NoDirection),
    _forceDirection(Geo::NoDirection),
    _complete(false),
    _routingMargin(DefaultRoutingMargin),
    _debugLevel(KLog::Debug)
{
}

Line::List PathRouter::calcluatePath()
{
    Point a = _originPoint;
    Point b = _destinationPoint;

    logText(KLOG_DEBUG, QString("Calculate path from %1 to %2 with %3 obstacles").arg(a.toString()).arg(b.toString()).arg(_obstacles.count()));

    try
    {
        pointLiesWithinObstacle(_originPoint, _originObstacle);
        pointLiesWithinObstacle(_destinationPoint, _destinationObstacle);

        int MAX_CYCLES = 500;
        // cycle on calculation
        int cycles = 0;
        do {
            cycle(a, b);

            // if the last two are the same, we have failed
            if(_pathLines.count() >= 2 && (_pathLines[_pathLines.count() - 2] == _pathLines[_pathLines.count() - 1])) {
                logText(KLOG_ERROR, "Failed to resolve");
                break;
            }
            a = _pathLines.last().p2();

            // prevent infinite looping
            if(cycles++ > MAX_CYCLES) {
                logText(KLOG_ERROR, "Abandoning after too many cycles");
                break;
            }
        }while(!_complete);
    }
    catch(const CommonException& e)
    {
        logText(KLOG_ERROR, QString(e.message()));
    }

    return _pathLines;
}

bool PathRouter::cycle(const Point &a, const Point &b)
{
    bool result = false;

    if( (result = pass1(a, b)) == false &&
        (result = pass2(a, b)) == false &&
        (result = pass3(a, b)) == false) {

    }
    return result;
}

/**
 * @brief PathRouter::pass1
 * This pass looks for a direct shot
 * @param a
 * @param b
 * @return
 */
bool PathRouter::pass1(const Point &a, const Point &b)
{
    bool result = false;

    QList<Rectangle> obstaclesToIgnore = { _originObstacle, _destinationObstacle };

    Rectangle obstacle;
    Point intersection;
    Line direct(a, b);

    if(direct.isPerpendicular() && lineCrossesObstacle(direct, obstaclesToIgnore, obstacle, intersection) == false) {
        appendPathLine(direct);
        _complete = true;      // done!
        result = true;
    }
    return result;
}

/**
 * @brief PathRouter::pass2
 * This pass chooses a direction, and if blocked, will try and get off the obstacle
 * @param a
 * @param b
 * @return
 */
bool PathRouter::pass2(const Point &a, const Point &b)
{
    bool result = false;

    // choose a direction

    if(_forceDirection == NoDirection) {
        Direction newDirection = chooseDirection(a, b, QList<Direction>() << _direction << oppositeDirection(_direction));
        if(newDirection != NoDirection) {
            _direction = newDirection;
        }
        else {
            // we have nowhere to go. We have probably landed on an obstacle
            Rectangle obstacle;
            if(pointLiesWithinObstacle(a, obstacle)) {
                Line pathOff = findPathOffObstacle(a, obstacle, newDirection);
                if(newDirection != NoDirection) {
                    appendPathLine(pathOff);
                    _direction = newDirection;
                    result = true;
                }
                else {
                    logText(KLOG_WARNING, "Failed to find path off obstacle");
                }
            }
        }
    }
    else {
        _direction = _forceDirection;
        _forceDirection = NoDirection;
    }
    return result;
}

bool PathRouter::pass3(const Point &a, const Point &b)
{
    bool result = false;

    // Detect obstacle in the way
    Line ray = rayInDirection(a, _direction);
    Rectangle obstacle;
    Point intersection;
    if(lineCrossesObstacle(ray, _originObstacle, obstacle, intersection)) {
        double distanceToB = distanceInDirection(a, _direction, b);
        double distanceToObstacle = FlatGeo::distance(a, intersection);

        // if the destination 'b' lies between our current point and the obstacle,
        // travel the distance to destination line
        if(distanceToB < distanceToObstacle) {
            Line newLine(a, _direction, distanceToB);
            appendPathLine(newLine);
            result = true;
        }
        else if(distanceToObstacle == 0) {
            // we are on an obstacle... get off of it
            Direction newDirection;
            Line pathOff = findPathOffObstacle(a, obstacle, newDirection);
            if(newDirection != NoDirection) {
                appendPathLine(pathOff);
                _direction = newDirection;
                result = true;
            }
            else {
                logText(KLOG_WARNING, "Failed to find path off obstacle (2)");
            }
        }
        else {
            Line lineToTarget(a, intersection);
            lineToTarget = reduceLineToTarget(lineToTarget, _direction, b, _routingMargin);

            // make another line to 'a' and intersection
            Line lineToIntersection(a, intersection);

            if(lineToTarget.length() <= lineToIntersection.length()) {
                // the line will rouch the obstacle.. try and shorten it
                if(lineToTarget.length() > _routingMargin) {
                    lineToTarget.shorten(_routingMargin).round();
                }
                _forceDirection = chooseDirection(a, b, directionOfLine(lineToTarget));
                appendPathLine(lineToTarget);
                result = true;
            }
            else {
                // go to the midpoint
                lineToIntersection = Line(lineToIntersection.p1(), lineToIntersection.midpoint());
                appendPathLine(lineToIntersection);
                result = true;
            }
        }
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
                result = true;
            }
            else {
                /**
                 * No completely clear path found. try again limiting the search distance to the
                 * distance to 'b' along the two best axes
                 */

                // get the two directions towards target and try to find a clear line along both
                QList<Direction> targetDirections = chooseDirections(a, b);

                for(Direction targetDirection : targetDirections) {
                    double distanceToB = distanceInDirection(a, targetDirection, b);

                    nextLine = seekClearLine(a, _direction, targetDirection, distanceToB);
                    if(nextLine.isValid()) {
                        appendPathLine(nextLine);
                        result = true;
                    }
                }
                if(result == false) {
                    logText(KLOG_DEBUG, "No clear line. WHAT NOW?");
                }
            }
        }
        else {
            // we are going the same direction as previous pass... go as far as possible
            // towards our target
            Line checkLine = rayInDirection(a, desiredDirection);
            if(lineCrossesObstacle(checkLine, _originObstacle, obstacle, intersection) == false) {
                // nothing in the way. go as far as possible towards target
                checkLine = reduceLineToTarget(checkLine, desiredDirection, b, 0);
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
    return result;
}

bool PathRouter::lineCrossesObstacle(const Line &line, Rectangle &firstObstacle, Point &intersection) const
{
    return lineCrossesObstacle(line, QList<Rectangle>(), firstObstacle, intersection);
}

bool PathRouter::lineCrossesObstacle(const Line &line, const Rectangle &ignoreObstacle, Rectangle &firstObstacle, Point &intersection) const
{
    return lineCrossesObstacle(line, QList<Rectangle>() << ignoreObstacle, firstObstacle, intersection);
}

bool PathRouter::lineCrossesObstacle(const Line& line, const QList<Rectangle> &ignoreObstacles, Rectangle &firstObstacle, Point& intersection) const
{
    bool result = false;
    double minDistance = INFINITY;
    try
    {
        for(const Rectangle& obstacle : _obstacles) {
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

bool PathRouter::pointLiesWithinObstacle(const Point& point, Rectangle &result)
{
    result = Rectangle();
    for(const Rectangle& obstacle : _obstacles) {
        Rectangle inflated = obstacle.adjusted(-1, -1, 1, 1);
        if(inflated.contains(point.toPoint())) {
            result = obstacle;
            break;
        }
    }
    return result.isNull() == false;
}

QList<Direction> PathRouter::chooseDirections(const Point &a, const Point &b)
{
    QList<Direction> result;
    result.append(chooseDirection(a, b));
    result.append(chooseDirection(a, b, result.at(0)));
    return result;
}

Geo::Direction PathRouter::chooseDirection(const Point &a, const Point &b, Direction exclude)
{
    return chooseDirection(a, b, QList<Direction>() << exclude);
}

Direction PathRouter::chooseDirection(const Point &a, const Point &b, const QList<Geo::Direction> exclude)
{
    Geo::Direction result = Geo::NoDirection;

    double dx = qAbs(a.x() - b.x());
    double dy = qAbs(a.y() - b.y());
    if(b.isRightOf(a)) {
        if(b.isAbove(a)) {
            result = dx < dy ? Up : ToRight;
            if(exclude.contains(result)) {
                result = (result == Up) ? ToRight : Up;
            }
        }
        else {
            result = dx < dy ? Down : ToRight;
            if(exclude.contains(result)) {
                result = (result == Down) ? ToRight : Down;
            }
        }
    }
    else if(b.isLeftOf(a)) {
        if(b.isAbove(a)) {
            result = dx < dy ? Up : ToLeft;
            if(exclude.contains(result)) {
                result = (result == Up) ? ToLeft : Down;
            }
        }
        else {
            result = dx < dy ? Down : ToLeft;
            if(exclude.contains(result)) {
                result = (result == Down) ? ToLeft : Down;
            }
        }
    }
    else {
        if(b.isAbove(a)) {
            result = Up;
            if(exclude.contains(result)) {
                result = NoDirection;
            }
        }
        else {
            result = Down;
            if(exclude.contains(result)) {
                result = NoDirection;
            }
        }
    }
    return result;
}

Direction PathRouter::nonExcludedDirection(QList<Geo::Direction> choices, QList<Geo::Direction> exclude)
{
    Direction result = NoDirection;
    for(Direction d : choices) {
        if(exclude.contains(d) == false) {
            result = d;
            break;
        }
    }
    return result;
}

Direction PathRouter::directionOfLine(const Line &line)
{
    Direction result = NoDirection;
    if(line.p1().x() == line.p2().x()) {
        result = line.p1().y() > line.p2().y() ? Up : Down;
    }
    else if(line.p1().y() == line.p2().y()) {
        result = line.p1().x() > line.p2().x() ? ToLeft : ToRight;
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
    result = Line(origin, other).round();
    return result;
}

Line PathRouter::seekClearLine(const Point &origin, Geo::Direction moveAxis, Direction seekAxis, double distanceLimit)
{
    Line result;
    Point nextOrigin = origin;

    QList<Rectangle> obstaclesToIgnore = { _destinationObstacle };

    while(_canvas.contains(nextOrigin)) {
        nextOrigin.move(moveAxis, _routingMargin);
        Line ray = rayInDirection(nextOrigin, seekAxis);
        if(distanceLimit != 0) {
            ray.shorten(ray.length() - distanceLimit);
            ray.round();
        }
        Rectangle obstacle;
        Point intersection;

        if(lineCrossesObstacle(ray, obstaclesToIgnore, obstacle, intersection) == false) {
            if(distanceLimit == 0) {
                result = Line(origin, nextOrigin);
            }
            else {
                result = Line(origin, ray.p2());
            }
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

double PathRouter::distanceInDirection(const Point &origin, Geo::Direction direction, const Point &target)
{
    Line line;
    line.setP1(origin);
    switch(direction) {
    case Up:
        if(target.y() < origin.y()) {
            line.setP2(Point(origin.x(), target.y()));
        }
        break;
    case Down:
        if(target.y() > origin.y()) {
            line.setP2(Point(origin.x(), target.y()));
        }
        break;
    case ToLeft:
        if(target.x() < origin.x()) {
            line.setP2(Point(target.x(), origin.y()));
        }
        break;
    case ToRight:
        if(target.x() > origin.x()) {
            line.setP2(Point(target.x(), origin.y()));
        }
        break;
    default:
        break;
    }
    return line.p2().isNull() ? INFINITY : line.length();
}

Line PathRouter::findPathOffObstacle(const Point &origin, const Rectangle &obstacle, Geo::Direction &direction)
{
    Line result;

    Point closest = obstacle.closestCorner(origin);
    direction = chooseDirection(origin, closest);
    result = Line(origin, closest);
    result.extend(_routingMargin);
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
