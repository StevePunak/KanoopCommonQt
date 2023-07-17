/**
 * pathrouter.cpp
 *
 * Works in multiple passes:
 *
 * Pass 1 -
 *   Resolve all the lines to get from source to destination without crossing any obstacles
 *     a) Find a direct shot from 'a' to diestination if it exists.
 *        This will yield the final result when successful.
 *
 *     b) Choose a direction for this segment
 *
 *     c) Find our way off of any obstacle whos edge we are currently on
 *
 *     d) Find the next point
 *
 * Pass 2 -
 *   Squash any segments which contain unncessary lines. That is, three consecutive sides which would
 *   form a rectangle if a fourth would be added can be turned into two lines so long as they don't
 *   intersect with obstacles.
 *
 *
 * Pass 3 -
 *   Try and apply the margin constraints. We will move any lines that fall on an obstacles edge off
 *   of that obstacle.
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
    _initialDirectionForced(false),
    _firstSegmentLength(0),
    _complete(false),
    _routingMargin(DefaultRoutingMargin),
    _consolidateEmptyRectangles(true),
    _routeAroundMargins(true),
    _verticalConstraint(-1),
    _debugLevel(KLog::Informational)
{
}

Line::List PathRouter::calcluatePath()
{
    _initialDirectionForced = _forceDirection != NoDirection;       // set flag if initial direction is forced

    pass1();
    pass2();
    pass3();
    return _pathLines;
}

bool PathRouter::pass1()
{
    bool result = false;
    Point a = _originPoint;
    Point b = _destinationPoint;

    logText(KLOG_DEBUG, QString("Pass 1 - Calculate path from %1 to %2 with %3 obstacles").arg(a.toString()).arg(b.toString()).arg(_obstacles.count()));

    try
    {
        pointLiesWithinObstacle(_originPoint, _originObstacle);
        pointLiesWithinObstacle(_destinationPoint, _destinationObstacle);

        int MAX_CYCLES = 500;
        // cycle on calculation
        int cycles = 0;
        do {
            cycleOnPass1(a, b);

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
        result = true;
    }
    catch(const CommonException& e)
    {
        logText(KLOG_ERROR, QString(e.message()));
        result = false;
    }
    return result;
}

/**
 * @brief PathRouter::pass2
 * Line / rectangle consolidation
 * @return
 */
bool PathRouter::pass2()
{
    logText(KLOG_DEBUG, QString("Pass 2 - Line Consolidation"));

    bool result = false;

    // 1. Remove zero-length lines
    removeZeroLengthLines(_pathLines);

    // 2. consolidate rectangular regions
    mergeAdjacentLines(_pathLines);
    if(_consolidateEmptyRectangles) {
        for(int i = 0;i < _pathLines.count() - 2;i++) {
            // if the initial direction is forced, we will not attempt to consolidate the first line
            if(i == 0 && _initialDirectionForced) {
                continue;
            }

            // try to make a rectangle from the next three lines
            Line::List nextThree = Line::List(_pathLines.mid(i, 3));
            Line consolidatedLine;
            Rectangle rect = makeRectangleFromThreeLines(nextThree, consolidatedLine);

            // if we were able to make a valid rectangle and it contains no objects, we can consolidate these lines
            if(rect.isValid()) {
                logText(KLOG_DEBUG, QString("Made valid rectangle [%1] with consolidated line [%2]").arg(rect.toString()).arg(consolidatedLine.toString()));
                QList<Rectangle> ignore;
                if(_obstacles.contains(rect)) {
                    ignore.append(rect);
                }

                if(rectangleCrossesObstacle(rect, ignore) == false) {
                    Line::List newLines;
                    newLines.append(Line(nextThree.at(0).p1(), consolidatedLine.p1()));
                    newLines.append(consolidatedLine);
                    replaceLines(i, 3, newLines);
                    i = qMax(0, i - 3);
                }
            }
        }
    }
    // remove any zero length lines introduced by recangle consolidation
    removeZeroLengthLines(_pathLines);

    // 3. Try and take out any segments smaller than the routing margin
    mergeTinySegments(_pathLines);

    // 4. consolidate adjacent lines which are on the same axis into a single line
    mergeAdjacentLines(_pathLines);

    return result;
}

/**
 * @brief PathRouter::pass3
 * Apply margins
 * @return
 */
bool PathRouter::pass3()
{
    logText(KLOG_DEBUG, QString("Pass 3 - Apply margins"));

    bool result = false;

    if(!_routeAroundMargins) {
        return result;
    }

    for(int i = 0;i < _pathLines.count();i++) {
        Line line = _pathLines.at(i);
        Rectangle obstacle;
        Line obstacleEdge;
        if(lineLiesOnObstacleEdge(line, obstacle, obstacleEdge)) {
            logText(KLOG_DEBUG, QString("Line [%1] lies on edge [%2] of obstacle [%3]. There are %4 adjacent lines")
                                    .arg(line.toString()).arg(obstacleEdge.toString()).arg(obstacle.toString()).arg(countAdjacentLines(_pathLines)));

            // get opposite direction of previous line. we will try and move in that direction by the margin
            if(i != 0) {
                Line previous = _pathLines.at(i - 1);

                // make sure previous is long enough
                if(previous.length() >= _routingMargin) {
                    Direction direction = directionOffObstacle(line, obstacle);
                    Line proposedNewLine = line;
                    proposedNewLine.move(direction, _routingMargin);

                    // if the proposed new line doesn't cross any obstacles, we're good to go with the proposed line
                    Rectangle obstacle;
                    Point intersection;
                    if(lineCrossesObstacle(proposedNewLine, obstacle, intersection) == false) {
                        Line::List newLines;
                        newLines.append(Line(previous.p1(), proposedNewLine.p1()));
                        newLines.append(proposedNewLine);
                        if(i < _pathLines.count() - 1) {
                            Line next = _pathLines.at(i + 1);
                            Line newNextLine(proposedNewLine.p2(), next.p2());
                            if(newNextLine.isPerpendicular() == false) {
                                logText(KLOG_ERROR, "Trying to insert a non-perpendicular line");
                            }
                            newLines.append(newNextLine);
                        }
                        removeZeroLengthLines(newLines);
                        mergeAdjacentLines(newLines);

                        replaceLines(i - 1, newLines.count(), newLines);

                        i--;    // back up
                    }
                    else {
                        // Our proposed new line has encountered at least one obstacle. We will attempt to route around it
                        double distance = distanceInDirection(proposedNewLine.p1(), proposedNewLine.direction(), intersection);
                        if(distance >= _routingMargin) {
                            Line originalLine = line;
                            Line proposedLineSegment(proposedNewLine.p1(), intersection);
                            Point endConnectorP2 = proposedLineSegment.p2().move(oppositeDirection(direction), _routingMargin);
                            Line endConnector(proposedLineSegment.p2(), endConnectorP2);
                            Line lastLine(endConnectorP2, originalLine.p2());

                            Line::List newLines;
                            newLines.append(Line(previous.p1(), proposedLineSegment.p1()));
                            newLines.append(proposedLineSegment);
                            newLines.append(endConnector);
                            newLines.append(lastLine);

                            // turn the old two lines into the new four lines
                            replaceLines(i - 1, 2, newLines);

                            i--;    // back up
                        }
                    }
                }

                // if we introduced adjacent lines, remove them and pop back for another pass
                int adjacentLines = countAdjacentLines(_pathLines);
                if(adjacentLines > 0) {
                    mergeAdjacentLines(_pathLines);
                    i = qMax(i - adjacentLines, 1);
                }
            }
        }
    }

    return result;
}

bool PathRouter::cycleOnPass1(const Point &a, const Point &b)
{
    bool result = false;

    if( (result = cycleFindDirectShot(a, b)) == false &&
        (result = cycleChooseDirection(a, b)) == false &&
        (result = cycleFindWayOffObstacle(a, b)) == false &&
        (result = cycleFindNextPoint(a, b)) == false) {
    }
    return result;
}

/**
 * @brief PathRouter::pass1
 * This pass looks for a direct shot
 * @param a
 * @param b
 * @return true if a line was added
 */
bool PathRouter::cycleFindDirectShot(const Point &a, const Point &b)
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
 * @brief PathRouter::cycleChooseDirection
 * This pass chooses a direction, and if blocked, will try and get off the obstacle
 * @param a
 * @param b
 * @return true if a line was added
 */
bool PathRouter::cycleChooseDirection(const Point &a, const Point &b)
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
            if(pointLiesOnObstacleEdge(a, obstacle)) {
                Line pathOff = findClosestCornerPathOffObstacle(a, obstacle, newDirection);
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

/**
 * @brief PathRouter::cycleFindWayOffObstacle
 * Find our way off any obstacle we are on
 * @param a
 * @param b
 * @return true if a line was added
 */
bool PathRouter::cycleFindWayOffObstacle(const Point &a, const Point &b)
{
    bool result = false;
    Rectangle obstacle;

    // get best directions to target, in order of preference
    QList<Direction> targetDirections = chooseDirections(a, b);

    // don't go back in the direction we are coming from
    Direction reverse = oppositeDirection(lastLineDirection());
    if(targetDirections.contains(reverse)) {
        targetDirections.removeOne(reverse);
    }

    if(pointLiesOnObstacleEdge(a, obstacle)) {
        if(obstacle == _destinationObstacle) {
            // If we are on the target obstacle, the remaining passes will get us to the interior (if necessary)
            logText(KLOG_DEBUG, QString("Point %1 lies on the destination obstacle").arg(a.toString()));
        }
        else {
            // if this point lies on a corner of the obstacle, and the corner is appropriate given
            // our target directions, we don't need to do anything.
            if(pointLiesOnAppropriateObtstacleCorner(a, obstacle, targetDirections)) {
                // nothing to do
            }
            else {
                // find a path off the obstacle in one of the valid directions
                Line pathOff = findPathOffObstacleInDirections(a, obstacle, targetDirections);
                if(pathOff.isValid()) {
                    appendPathLine(pathOff);
                    _direction = pathOff.direction();
                    result = true;
                }
            }

        }
    }
    return result;
}

/**
 * @brief PathRouter::cycleFindNextPoint
 *
 * Find the next point. This is the main workhorse logic.
 *
 * @param a
 * @param b
 * @return true if a line was added
 */
bool PathRouter::cycleFindNextPoint(const Point &a, const Point &b)
{
    bool result = false;

    // Detect obstacle in the way. We will ignore any obstacle we may be on
    // since getting off of it should be taken care of in the previous pass
    Rectangle::List obstaclesToIgnore;
    if(_originObstacle.isValid()) {
        obstaclesToIgnore.append(_originObstacle);
    }
    Rectangle currentObstacle;
    if(pointLiesOnObstacleEdge(a, currentObstacle)) {
        obstaclesToIgnore.append(currentObstacle);
    }

    Line ray = rayInDirection(a, _direction);
    Rectangle obstacle;
    Point intersection;
    if(lineCrossesObstacle(ray, obstaclesToIgnore, obstacle, intersection)) {
        double distanceToB = distanceInDirection(a, _direction, b);
        double distanceToObstacle = FlatGeo::distance(a, intersection);

        // if the destination 'b' lies between our current point and the obstacle,
        // travel the distance to destination line
        if(distanceToB < distanceToObstacle) {
            Line newLine(a, _direction, distanceToB);
            newLine.round();
            appendPathLine(newLine);
            result = true;
        }
        else if(distanceToObstacle == 0) {
            // we are on an obstacle... get off of it
            Direction newDirection;
            Line pathOff = findClosestCornerPathOffObstacle(a, obstacle, newDirection);
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
            lineToTarget = reduceLineToTarget(lineToTarget, _direction, b, 0);

            // make another line to 'a' and intersection
            Line lineToIntersection(a, intersection);

            if(lineToTarget.length() <= lineToIntersection.length()) {
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
            if(lineCrossesObstacle(checkLine, obstaclesToIgnore, obstacle, intersection) == false) {
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

bool PathRouter::rectangleCrossesObstacle(const Rectangle &rectangle, const QList<Rectangle> &ignoreObstacles) const
{
    bool result = false;
    for(const Rectangle& obstacle : _obstacles) {
        if(ignoreObstacles.contains(obstacle)) {
            continue;
        }

        if(rectangle.intersects(obstacle)) {
            result = true;
            break;
        }
    }
    return result;
}

bool PathRouter::lineCrossesObstacle(const Line &line) const
{
    Rectangle obstacle;
    Point intersection;
    return lineCrossesObstacle(line, obstacle, intersection);
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

bool PathRouter::pointLiesOnObstacleEdge(const Point &point, Rectangle &result)
{
    result = Rectangle();
    for(const Rectangle& obstacle : _obstacles) {
        if(obstacle.isPointOnEdge(point)) {
            result = obstacle;
            break;
        }
    }
    return result.isNull() == false;
}

bool PathRouter::lineLiesOnObstacleEdge(const Line &line, Rectangle &foundObstacle, Line &foundEdge)
{
    foundObstacle = Rectangle();
    for(const Rectangle& obstacle : _obstacles) {
        Line::List edges = line.isVertical() ? obstacle.verticalLines() : obstacle.horizontalLines();
        for(const Line& edge : edges) {
            if(line.containsPoint(edge.p1()) || line.containsPoint(edge.p2())) {
                foundObstacle = obstacle;
                foundEdge = edge;
                break;
            }
        }
        if(foundObstacle.isEmpty() == false) {
            break;
        }
    }
    return foundObstacle.isNull() == false;
}

/**
 * @brief PathRouter::consolidateLines
 * The three lines at 'index' will be turned into a single line
 * @param index
 * @param consolidationRectangle
 */
void PathRouter::replaceLines(int index, int count, const Line::List& newLines)
{
    _pathLines.remove(index, count);
    for(const Line& line : newLines) {
        _pathLines.insert(index++, line);
    }
}

Direction PathRouter::directionOffObstacle(const Line &line, const Rectangle &obstacle)
{
    Direction result = NoDirection;

    QList<Direction> choices;

    if(line.isHorizontal()) {
        choices.append(Up);
        choices.append(Down);
    }
    else {
        choices.append(ToLeft);
        choices.append(ToRight);
    }

    for(Direction direction : choices) {
        Line testLine = line;
        testLine.move(direction, 1);
        if(obstacle.containsAnyPoint(testLine) == false) {
            result = direction;
            break;
        }
    }
    return result;
}

QList<Direction> PathRouter::chooseDirections(const Point &a, const Point &b)
{
    QList<Direction> result;
    result.append(chooseDirection(a, b));
    result.append(chooseDirection(a, b, result.at(0)));
    result.removeAll(NoDirection);
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

Rectangle PathRouter::makeRectangleFromThreeLines(const Line::List &lines, Line& newLine) const
{
    Rectangle result;
    if(lines.count() != 3) {
        return result;
    }

    QList<Direction> directions;
    for(int i = 0;i < lines.count();i++) {
        if(directions.contains(lines.at(i).direction()) == false) {
            directions.append(lines.at(i).direction());
        }
    }

    // must be three unique directions
    if(directions.count() == 3) {

        // Get the direction to try for our rectangle
        Direction finalDirection = NoDirection;
        for(Direction d : allDirections()) {
            if(directions.contains(d) == false) {
                finalDirection = d;
                break;
            }
        }

        if(finalDirection != NoDirection) {
            Line line1 = lines.at(0);
            Line line2 = lines.at(1);
            Line line3 = lines.at(2);
            Line line = rayInDirection(line3.p2(), finalDirection);
            Point intersection;
            if(line.intersects(line1, intersection)) {
                Point::List points;
                points.append(intersection);
                points.append(line1.p2());
                points.append(line2.p2());
                points.append(line3.p2());

                Rectangle rectangle = Rectangle::fromPoints(points);
                if(rectangle.isValid()) {
                    result = rectangle;
                    newLine = Line(intersection, line3.p2());
                }
            }

        }
    }
    return result;
}

Line PathRouter::rayInDirection(const Point &origin, Geo::Direction direction) const
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

    bool firstSegmentDistanceApplied = false;
    while(_canvas.contains(nextOrigin)) {
        double seekDistance = _routingMargin;
        // here we will try and apply the first segment length as given by the caller
        if(_firstSegmentLength != 0 && _pathLines.count() == 0 && firstSegmentDistanceApplied == false) {
            seekDistance = _firstSegmentLength;
            firstSegmentDistanceApplied = true;
        }

        nextOrigin.move(moveAxis, seekDistance);
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

double PathRouter::distanceInDirection(const Point &origin, Geo::Direction direction, const Point &target) const
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

Line PathRouter::findClosestCornerPathOffObstacle(const Point &origin, const Rectangle &obstacle, Geo::Direction &resultDirection) const
{
    Line result;

    Point closest = obstacle.closestCorner(origin);
    resultDirection = chooseDirection(origin, closest);
    result = Line(origin, closest);
    return result;
}

/**
 * @brief PathRouter::findPathOffObstacleInDirections
 * Find the shortest path off the obstacle in one of the given directions
 * @param origin
 * @param obstacle
 * @param directions  - The possible directions on which to search
 * @return The resultant line off the obstacle
 */
Line PathRouter::findPathOffObstacleInDirections(const Point &origin, const Rectangle &obstacle, const QList<Geo::Direction> &directions) const
{
    Line result;
    Point::List corners = obstacle.corners();
    for(const Point& corner : corners) {
        Line line(origin, corner);
        if(directions.contains(line.direction()) == false) {
            continue;
        }
        if(result.isValid() == false || line.length() < result.length()) {
            result = line;
        }
    }
    return result;
}

bool PathRouter::pointLiesOnAppropriateObtstacleCorner(const Point &origin, const Rectangle &obstacle, const QList<Geo::Direction> &directions) const
{
    bool result = false;
    Point::List corners = obstacle.corners();
    if(corners.contains(origin)) {
        if(origin == obstacle.topRight() && (directions.contains(Up) || directions.contains(ToRight))) {
            result = true;
        }
        else if(origin == obstacle.topLeft() && (directions.contains(Up) || directions.contains(ToLeft))) {
            result = true;
        }
        else if(origin == obstacle.bottomLeft() && (directions.contains(Down) || directions.contains(ToLeft))) {
            result = true;
        }
        else if(origin == obstacle.bottomRight() && (directions.contains(Down) || directions.contains(ToRight))) {
            result = true;
        }
    }
    return result;
}

Direction PathRouter::lastLineDirection() const
{
    Direction result = NoDirection;
    if(_pathLines.count() > 0) {
        result = _pathLines.last().direction();
    }
    return result;
}

void PathRouter::removeZeroLengthLines(Line::List &lines) const
{
    for(int i = 0;i < lines.count();i++) {
        if(lines.at(i).length() == 0) {
            lines.removeAt(i);
            --i;
        }
    }
}

void PathRouter::mergeAdjacentLines(Line::List &lines) const
{
    for(int i = 0;i < lines.count() - 1;i++) {
        Line thisLine = lines.at(i);

        Line::List linesToMerge;
        int j = i + 1;
        for(;j < lines.count();j++) {
            Line nextLine = lines.at(j);
            if(thisLine.sharesAxisWith(nextLine)) {
                linesToMerge.append(nextLine);
            }
            else {
                break;
            }
        }
        if(linesToMerge.count() > 0) {
            linesToMerge.prepend(thisLine);

            // the merged line needs to go in the original direction
            Direction direction = thisLine.direction();
            int startX = -1;
            int startY = -1;
            int endX = -1;
            int endY = -1;
            if(thisLine.isHorizontal()) {
                startY = endY = thisLine.p1().y();
            }
            else if(thisLine.isVertical()) {
                startX = endX = thisLine.p1().x();
            }

            switch(direction) {
            case ToLeft:
                startX = linesToMerge.maxX();
                endX = linesToMerge.minX();
                break;
            case ToRight:
                startX = linesToMerge.minX();
                endX = linesToMerge.maxX();
                break;
            case Up:
                startY = linesToMerge.maxY();
                endY = linesToMerge.minY();
                break;
            case Down:
                startY = linesToMerge.minY();
                endY = linesToMerge.maxY();
                break;
            default:
                break;
            }

            Line newLine(Point(startX, startY), Point(endX, endY));

            // our new line must end up with the start point of the next line, if present
            if(j < lines.count()) {
                Line nextLine = lines.at(j);
                if(newLine.p2() != nextLine.p1()) {
                    logText(KLOG_DEBUG, QString("Shortening new line [%1] to meet up with next endpoint at [%2]").arg(newLine.toString()).arg(nextLine.p1().toString()));
                    newLine.setP2(nextLine.p1());
                }
            }
            lines.remove(i, linesToMerge.count());
            lines.insert(i, newLine);
        }
    }
}

/**
 * @brief PathRouter::mergeTinySegments
 * Try and merge any small jogs
 * @param lines
 */
void PathRouter::mergeTinySegments(Line::List &lines) const
{
    for(int i = 1;i < lines.count() - 2;i++) {
        Line seg1 = lines.at(i);
        Line seg2 = lines.at(i + 1);
        Line seg3 = lines.at(i + 2);

        // In order to do this, the first and third segments must be going in the same
        // direction with the middle segment being a jog smaller than routing margin.
        if(seg1.direction() == seg3.direction() && seg2.length() < _routingMargin) {
            Line proposedNewLine = seg1;
            proposedNewLine.move(seg2.direction(), seg2.length() / 2);
            double extendBy = distanceInDirection(proposedNewLine.p2(), proposedNewLine.direction(), seg3.p2());
            proposedNewLine.extend(extendBy);

        }
    }
}

bool PathRouter::lineCrossesVerticalConstraint(const Line &line)
{
    return _verticalConstraint >= 0 &&
        line.crossesX(_verticalConstraint) &&
        line.p1().x() != _verticalConstraint &&
           line.p2().x() != _verticalConstraint;
}

int PathRouter::countAdjacentLines(const Line::List &lines)
{
    int result = 0;
    for(int i = 0;i < lines.count() - 1;i++) {
        Line thisLine = lines.at(i);
        Line nextLine = lines.at(i + 1);

        if(thisLine.sharesAxisWith(nextLine)) {
            ++result;
        }
    }
    return result;
}

void PathRouter::appendPathLine(const Line &line)
{
    Line constrainedLine = line;
    logText(KLOG_DEBUG, QString("Appending path line %1").arg(constrainedLine.toString()));
    if(lineCrossesVerticalConstraint(constrainedLine)) {
        constrainedLine.setP2(Point(_verticalConstraint, constrainedLine.p2().y()));
        logText(KLOG_DEBUG, QString("Constraining path line vertically from %1 to %2").arg(line.toString()).arg(constrainedLine.toString()));
    }

    if(constrainedLine.isPerpendicular() == false) {
        logText(KLOG_ERROR, "Appending non-perpendicular line!");
    }
    _pathLines.append(constrainedLine);
}

void PathRouter::dumpPathLines(const QString& label) const
{
    logText(KLOG_DEBUG, QString("Dumping path lines for %1").arg(label));
    for(const Line& line : _pathLines) {
        logText(KLOG_DEBUG, QString("Path line %1").arg(line.toString()));
    }
}

void PathRouter::logText(const char *file, int line, int level, const QString &text) const
{
    if(level <= _debugLevel) {
        KLog::sysLogText(file, line, (KLog::LogLevel)level, text);
    }
}
