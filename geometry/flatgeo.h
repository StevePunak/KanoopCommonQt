#ifndef FLATGEO_H
#define FLATGEO_H

#include <QPointF>
#include <QRect>

class Circle;
class Line;
class Angle;
class QRect;

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

enum SpatialRelationship {
    NoRelationship              = 0x0000,
    IntersectsWith              = 0x0001,
    Contains                    = 0x0002,
    ContainedBy                 = 0x0004,
    AwayFrom                    = 0x0008,
    Towards                     = 0x0010,
    Above                       = 0x0020,
    Below                       = 0x0040,
    ToLeftOf                    = 0x0080,
    ToRightOf                   = 0x0100,
    Contained                   = 0x0200,
};

QList<Direction> allDirections();
Side directionToSide(Direction direction);
Direction sideToDirection(Side side);

}   // namespace Geo


class FlatGeo
{
public:
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
    static inline bool isRectLeftOf(const QRect& rect, const QPoint& other)
    {
        return isRectLeftOf(QRectF(rect), QPointF(other));
    }
    static inline bool isRectLeftOf(const QRectF& rect, const QPointF& other)
    {
        return rect.x() + rect.width() < other.x();
    }
    static inline bool isRectRightOf(const QRect& rect, const QPoint& other)
    {
        return isRectRightOf(QRectF(rect), QPointF(other));
    }
    static inline bool isRectRightOf(const QRectF& rect, const QPointF& other)
    {
        return rect.x() > other.x();
    }
    static inline bool isRectAbove(const QRect& rect, const QPoint& other)
    {
        return isRectAbove(QRectF(rect), QPointF(other));
    }
    static inline bool isRectAbove(const QRectF& rect, const QPointF& other)
    {
        return rect.y() + rect.height() < other.y();
    }
    static inline bool isRectBelow(const QRect& rect, const QPoint& other)
    {
        return isRectBelow(QRectF(rect), QPointF(other));
    }
    static inline bool isRectBelow(const QRectF& rect, const QPointF& other)
    {
        return rect.y() > other.y();
    }
    Geo::SpatialRelationship relationTo(const QPoint& origin, const QPoint& other)
    {
        return relationTo(QPointF(origin), QPointF(other));
    }
    Geo::SpatialRelationship relationTo(const QPointF& origin, const QPointF& other);
    Geo::SpatialRelationship relationTo(const QRect& origin, const QPoint& other)
    {
        return relationTo(QRectF(origin), QPointF(other));
    }
    Geo::SpatialRelationship relationTo(const QRectF& origin, const QPointF& other);

    static QString makePointString(const QPoint& p);
    static QString makePointString(const QPointF& p);

private:
    static Angle anglev2(const Line& l1, const Line& l2);
    static QString roundedString(double value, int precision);
};


#endif // FLATGEO_H



