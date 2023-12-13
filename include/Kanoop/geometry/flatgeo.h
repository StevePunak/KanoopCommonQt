#ifndef FLATGEO_H
#define FLATGEO_H

#include "point.h"
#include <QRect>

class Circle;
class Line;
class Angle;
class QRect;

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
    static Point getPoint(const QPointF& from, double bearing, double distance);
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
    static inline bool isRectLeftOf(const QRectF& rect, const QPointF& other)
    {
        return rect.x() + rect.width() < other.x();
    }
    static inline bool isRectRightOf(const QRectF& rect, const QPointF& other)
    {
        return rect.x() > other.x();
    }
    static inline bool isRectAbove(const QRectF& rect, const QPointF& other)
    {
        return rect.y() + rect.height() < other.y();
    }
    static inline bool isRectBelow(const QRectF& rect, const QPointF& other)
    {
        return rect.y() > other.y();
    }
    static inline bool isRectLeftOf(const QRectF& rect, const QRectF& other)
    {
        return other.x() < rect.x();
    }
    static inline bool isRectRightOf(const QRectF& rect, const QRectF& other)
    {
        return other.x() > rect.x() + rect.width();
    }
    static inline bool isRectAbove(const QRectF& rect, const QRectF& other)
    {
        return other.y() < rect.y();
    }
    static inline bool isRectBelow(const QRectF& rect, const QRectF& other)
    {
        return other.y() > rect.y() + rect.height();
    }
    static Geo::SpatialRelationship relationTo(const QPointF& origin, const QPointF& other);
    static Geo::SpatialRelationship relationTo(const QRectF& origin, const QPointF& other);
    static Geo::SpatialRelationship relationTo(const QPointF& origin, const QRectF& other);
    static Geo::SpatialRelationship relationTo(const QRectF& origin, const QRectF& other);

    static QString makePointString(const QPoint& p);
    static QString makePointString(const QPointF& p);

private:
    static Angle anglev2(const Line& l1, const Line& l2);
    static QString roundedString(double value, int precision);
};


#endif // FLATGEO_H



