#include "pathrouter.h"
#include "klog.h"

#include "commonexception.h"

PathRouter::PathRouter(const QPoint &origin, const QPoint &destination, const QList<QRect> &obstacles) :
    _originPoint(origin), _destinationPoint(destination),
    _obstacles(obstacles),
    _debugLevel(KLog::Debug)
{
}

Line::List PathRouter::calcluatePath()
{
    recursiveCalculatePath(_originPoint, _destinationPoint);
}

void PathRouter::recursiveCalculatePath(const Point &a, const Point &b)
{
    Line line(a, b);

}

bool PathRouter::lineCrossesObstacle(const Line& line, QRect &firstObstacle) const
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
                QPointF intersection;
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

    int dx = qAbs(b.x() - a.x());
    int dy = qAbs(b.y() - a.y());
    if(FlatGeo::isPointRightOf(a, b) && FlatGeo::isPointRightOf(a, b) && ) {

    }
}

void PathRouter::debugLog(const char *file, int line, int level, const QString &text)
{
    if(level <= _debugLevel) {
        KLog::sysLogText(file, line, (KLog::LogLevel)level, text);
    }
}
