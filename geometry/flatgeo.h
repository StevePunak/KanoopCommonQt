#ifndef FLATGEO_H
#define FLATGEO_H

#include <QPointF>

class Circle;
class Line;
class Angle;
class FlatGeo
{
public:
    enum Relationship {
        NoRelation = 0,
        IntersectsWith = 1,
        Contains = 2,
        ContainedBy = 3,
        AwayFrom = 4,
        Towards = 5
    };

    static double vectorAngle(const Line& l1, const Line& l2);
    static Angle angle(const Line& l1, const Line& l2);
    static Angle angle(const QPointF& p1, const QPointF& vertex, const QPointF& p2);
    static double distance(const QPointF& p1, const QPointF& p2);
    static double degrees(double angle);
    static double radians(double angle);
    static double angularDifference(const Line& l1, const Line& l2);
    static QPointF getPoint(const QPointF& from, double bearing, double distance);
    static QPointF intersection(const Line& l1, const Line& l2);
    static bool isInCircle(const QPointF& point, const Circle& circle);
    static void getLineABC(const Line& line, double& a, double& b, double& c);
    static QPointF move(const QPointF& point, double bearing, double distance);
    static QPointF rotate(const QPointF& point, const QPointF& centroid, double angle);
    static bool arePointsEqual(const QPointF& p1, const QPointF& p2, int precision = 0);
    static inline bool isPointLeftOf(const QPointF& point, const QPointF& other)
    {
        return point.x() < other.x();
    }
    static inline bool isPointRightOf(const QPointF& point, const QPointF& other)
    {
        return point.x() > other.x();
    }
    static inline bool isPointAbove(const QPointF& point, const QPointF& other)
    {
        return point.y() < other.y();
    }
    static inline bool isPointBelow(const QPointF& point, const QPointF& other)
    {
        return point.y() > other.y();
    }

    static QString makePointString(const QPoint& p);
    static QString makePointString(const QPointF& p);

private:
    static Angle anglev2(const Line& l1, const Line& l2);
    static QString roundedString(double value, int precision);
};

namespace Geo {

enum Side
{
    NoSide,
    Top, Left, Bottom, Right,
    TopLeftCorner, TopRightCorner,
    BottomLeftCorner, BottomRightCorner,
};

enum Direction
{
    NoDirection = NoSide,
    Up = Top,
    Down = Bottom,
    ToLeft = Left,
    ToRight = Right
};

QList<Direction> allDirections();
Side directionToSide(Direction direction);
Direction sideToDirection(Side side);

}   // namespace Geo


#endif // FLATGEO_H



