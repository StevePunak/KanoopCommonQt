#ifndef PATHROUTER_H
#define PATHROUTER_H

#include "line.h"

#include <QList>
#include <QRect>
#include <QString>

#include "point.h"
#include "rectangle.h"
#include "Kanoop/kanoopcommon.h"

/**
 * @brief Orthogonal path-routing algorithm that navigates around rectangular obstacles.
 *
 * PathRouter computes a list of horizontal/vertical line segments connecting an origin
 * point to a destination point while avoiding a set of rectangular obstacles on a canvas.
 * Call calcluatePath() after configuration to obtain the routed Line::List.
 */
class KANOOP_EXPORT PathRouter
{
public:
    /** @brief Default constructor — creates an unconfigured router with default margin settings. */
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

    /**
     * @brief Construct a PathRouter with full configuration.
     * @param origin Starting point for the path
     * @param destination Ending point for the path
     * @param canvas Bounding rectangle for the entire routing area
     * @param obstacles List of rectangular obstacles to route around (default empty)
     */
    PathRouter(const Point& origin, const Point& destination, const QRectF &canvas, const QList<Rectangle> &obstacles = QList<Rectangle>());

    /**
     * @brief Compute the routed path from origin to destination avoiding all obstacles.
     * @return Ordered list of line segments forming the routed path
     */
    Line::List calcluatePath();

    /**
     * @brief Append a single obstacle rectangle to the routing obstacle set.
     * @param value Obstacle rectangle to add
     */
    void appendObstacle(const Rectangle& value) { _obstacles.append(value); }

    /**
     * @brief Replace the entire obstacle set.
     * @param value New list of obstacle rectangles
     */
    void setObstacles(const QList<Rectangle>& value) { _obstacles = value; }

    /**
     * @brief Return the origin point.
     * @return Current origin Point
     */
    Point originPoint() const { return _originPoint; }

    /**
     * @brief Set the origin point.
     * @param value New origin Point
     */
    void setOriginPoint(const Point& value) { _originPoint = value ; }

    /**
     * @brief Return the destination point.
     * @return Current destination Point
     */
    Point destinationPoint() const { return _destinationPoint; }

    /**
     * @brief Set the destination point.
     * @param value New destination Point
     */
    void setDestinationPoint(const Point& value) { _destinationPoint = value; }

    /**
     * @brief Return the routing margin applied around each obstacle.
     * @return Margin in pixels/units
     */
    int routingMargin() const { return _routingMargin; }

    /**
     * @brief Set the routing margin applied around each obstacle.
     * @param value Margin in pixels/units
     */
    void setRoutingMargin(int value) { _routingMargin = value; }

    /**
     * @brief Return whether empty (non-obstacle) rectangles are consolidated before routing.
     * @return true if consolidation is enabled
     */
    bool consolidateEmptyRectangles() const { return _consolidateEmptyRectangles; }

    /**
     * @brief Enable or disable consolidation of empty rectangles before routing.
     * @param value true to enable consolidation
     */
    void setConsolidateEmptyRectangles(bool value) { _consolidateEmptyRectangles = value; }

    /**
     * @brief Return whether the router routes around obstacle margins.
     * @return true if margin-based routing is enabled
     */
    bool routeAroundMargins() const { return _routeAroundMargins; }

    /**
     * @brief Enable or disable routing around obstacle margins.
     * @param value true to route around margins
     */
    void setRouteAroundMargins(bool value) { _routeAroundMargins = value; }

    /**
     * @brief Force the direction of the first routed segment.
     * @param direction Preferred initial travel direction
     */
    void setInitialDirection(Geo::Direction direction) { _forceDirection = direction; }

    /**
     * @brief Set the length of the first segment before obstacle avoidance begins.
     * @param value Length in pixels/units
     */
    void setFirstSegmentLength(double value) { _firstSegmentLength = value; }

    /**
     * @brief Return the vertical constraint (maximum Y coordinate the path may reach).
     * @return Constraint Y value, or -1 if unconstrained
     */
    int verticalConstraint() const { return _verticalConstraint; }

    /**
     * @brief Set the vertical constraint (maximum Y the path may reach).
     * @param value Y constraint value, or -1 to disable
     */
    void setVerticalConstraint(int value) { _verticalConstraint = value; }

    /**
     * @brief Return the debug verbosity level.
     * @return Debug level (0 = silent)
     */
    int debugLevel() const { return _debugLevel; }

    /**
     * @brief Set the debug verbosity level.
     * @param value Debug level (0 = silent, higher = more verbose)
     */
    void setDebugLevel(int value) { _debugLevel = value; }

    /**
     * @brief Merge adjacent collinear line segments in-place.
     * @param lines Line list to process; merged in place
     */
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
