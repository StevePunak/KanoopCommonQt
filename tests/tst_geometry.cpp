#include <QTest>
#include <QtMath>

#include <Kanoop/geometry/angle.h>
#include <Kanoop/geometry/circle.h>
#include <Kanoop/geometry/flatgeo.h>
#include <Kanoop/geometry/geo.h>
#include <Kanoop/geometry/line.h>
#include <Kanoop/geometry/point.h>
#include <Kanoop/geometry/polygon.h>
#include <Kanoop/geometry/rectangle.h>
#include <Kanoop/geometry/triangle.h>

static constexpr double TOL = 0.001;

static bool fuzzyEqual(double a, double b, double tolerance = TOL)
{
    return qAbs(a - b) <= tolerance;
}

class TstGeometry : public QObject
{
    Q_OBJECT

private slots:

    // ---------------------------------------------------------------
    // Angle
    // ---------------------------------------------------------------

    void angle_defaultIsInvalid()
    {
        Angle a;
        QCOMPARE(a.degrees(), -1.0);
    }

    void angle_constructFromDegrees()
    {
        Angle a(45.0);
        QVERIFY(fuzzyEqual(a.degrees(), 45.0));
    }

    void angle_additionWrapsAt360()
    {
        Angle a(350.0);
        Angle result = a + 20.0;
        QVERIFY(fuzzyEqual(result.degrees(), 10.0));
    }

    void angle_subtractionWrapsAtZero()
    {
        Angle a(10.0);
        Angle result = a - 20.0;
        QVERIFY(fuzzyEqual(result.degrees(), 350.0));
    }

    void angle_addAngleObject()
    {
        Angle a(100.0);
        Angle b(270.0);
        Angle result = a + b;
        QVERIFY(fuzzyEqual(result.degrees(), 10.0));
    }

    void angle_subtractAngleObject()
    {
        Angle a(30.0);
        Angle b(50.0);
        Angle result = a - b;
        QVERIFY(fuzzyEqual(result.degrees(), 340.0));
    }

    void angle_compoundAssignAdd()
    {
        Angle a(350.0);
        a += 30.0;
        QVERIFY(fuzzyEqual(a.degrees(), 20.0));
    }

    void angle_compoundAssignSubtract()
    {
        Angle a(5.0);
        a -= 10.0;
        QVERIFY(fuzzyEqual(a.degrees(), 355.0));
    }

    void angle_toString()
    {
        Angle a(90.0);
        QVERIFY(a.toString().contains("90"));
        QVERIFY(a.toString().contains("deg"));
    }

    // ---------------------------------------------------------------
    // Point
    // ---------------------------------------------------------------

    void point_defaultAtOrigin()
    {
        Point p;
        QVERIFY(fuzzyEqual(p.x(), 0.0));
        QVERIFY(fuzzyEqual(p.y(), 0.0));
    }

    void point_constructFromXY()
    {
        Point p(3.5, 7.2);
        QVERIFY(fuzzyEqual(p.x(), 3.5));
        QVERIFY(fuzzyEqual(p.y(), 7.2));
    }

    void point_constructFromQPointF()
    {
        QPointF qp(10.0, 20.0);
        Point p(qp);
        QVERIFY(fuzzyEqual(p.x(), 10.0));
        QVERIFY(fuzzyEqual(p.y(), 20.0));
    }

    void point_isLeftOfRightOfAboveBelow()
    {
        Point a(1.0, 5.0);
        Point b(3.0, 2.0);

        QVERIFY(a.isLeftOf(b));
        QVERIFY(!a.isRightOf(b));
        QVERIFY(!a.isAbove(b));   // a.y=5 > b.y=2 so a is below b
        QVERIFY(a.isBelow(b));
    }

    void point_moveDelta()
    {
        Point p(10.0, 20.0);
        p.moveDelta(5.0, -3.0);
        QVERIFY(fuzzyEqual(p.x(), 15.0));
        QVERIFY(fuzzyEqual(p.y(), 17.0));
    }

    void point_offset()
    {
        Point p(1.0, 2.0);
        p.offset(10.0, 20.0);
        QVERIFY(fuzzyEqual(p.x(), 11.0));
        QVERIFY(fuzzyEqual(p.y(), 22.0));
    }

    void point_delta()
    {
        // delta returns (this - other)
        Point a(4.0, 6.0);
        Point b(1.0, 2.0);
        Point d = a.delta(b);
        QVERIFY(fuzzyEqual(d.x(), 3.0));
        QVERIFY(fuzzyEqual(d.y(), 4.0));
    }

    void point_round()
    {
        Point p(3.7, 4.2);
        p.round();
        QVERIFY(fuzzyEqual(p.x(), 4.0));
        QVERIFY(fuzzyEqual(p.y(), 4.0));
    }

    void point_toStringAndFromString()
    {
        Point p(12.5, 34.5);
        QString s = p.toString();
        Point parsed = Point::fromString(s);
        QVERIFY(fuzzyEqual(parsed.x(), p.x()));
        QVERIFY(fuzzyEqual(parsed.y(), p.y()));
    }

    void point_moveByDirection()
    {
        Point p(0.0, 0.0);
        p.move(Geo::ToRight, 10.0);
        QVERIFY(fuzzyEqual(p.x(), 10.0));
        QVERIFY(fuzzyEqual(p.y(), 0.0));
    }

    void point_listCorners()
    {
        Point::List list;
        list.append(Point(0, 0));
        list.append(Point(10, 0));
        list.append(Point(10, 10));
        list.append(Point(0, 10));

        Point tl = list.topLeft();
        Point br = list.bottomRight();
        QVERIFY(fuzzyEqual(tl.x(), 0.0));
        QVERIFY(fuzzyEqual(tl.y(), 0.0));
        QVERIFY(fuzzyEqual(br.x(), 10.0));
        QVERIFY(fuzzyEqual(br.y(), 10.0));
    }

    // ---------------------------------------------------------------
    // Line
    // ---------------------------------------------------------------

    void line_defaultIsInvalid()
    {
        Line l;
        QVERIFY(!l.isValid());
    }

    void line_constructFromTwoPoints()
    {
        Line l(QPointF(0, 0), QPointF(3, 4));
        QVERIFY(fuzzyEqual(l.length(), 5.0));
    }

    void line_midpoint()
    {
        Line l(QPointF(0, 0), QPointF(10, 10));
        Point mid = l.midpoint();
        QVERIFY(fuzzyEqual(mid.x(), 5.0));
        QVERIFY(fuzzyEqual(mid.y(), 5.0));
    }

    void line_slopeHorizontal()
    {
        Line l(QPointF(0, 5), QPointF(10, 5));
        QVERIFY(fuzzyEqual(l.slope(), 0.0));
        QVERIFY(l.isHorizontal());
        QVERIFY(!l.isVertical());
    }

    void line_slopeVertical()
    {
        Line l(QPointF(5, 0), QPointF(5, 10));
        QVERIFY(l.isVertical());
        QVERIFY(!l.isHorizontal());
    }

    void line_bearing()
    {
        // Bearing 0 = north (up, -Y direction in screen coords)
        // A line going straight right should have bearing ~90
        Line l(QPointF(0, 0), QPointF(10, 0));
        QVERIFY(fuzzyEqual(l.bearing(), 90.0));
    }

    void line_bearingDown()
    {
        // Straight down => bearing 180
        Line l(QPointF(0, 0), QPointF(0, 10));
        QVERIFY(fuzzyEqual(l.bearing(), 180.0));
    }

    void line_intersectionCross()
    {
        Line l1(QPointF(0, 5), QPointF(10, 5));
        Line l2(QPointF(5, 0), QPointF(5, 10));
        QVERIFY(l1.intersects(l2));
        Point inter;
        l1.intersects(l2, inter);
        QVERIFY(fuzzyEqual(inter.x(), 5.0));
        QVERIFY(fuzzyEqual(inter.y(), 5.0));
    }

    void line_noIntersectionParallel()
    {
        Line l1(QPointF(0, 0), QPointF(10, 0));
        Line l2(QPointF(0, 5), QPointF(10, 5));
        QVERIFY(!l1.intersects(l2));
    }

    void line_containsPoint()
    {
        Line l(QPointF(0, 0), QPointF(10, 0));
        QVERIFY(l.containsPoint(QPointF(5, 0)));
        QVERIFY(!l.containsPoint(QPointF(5, 1)));
    }

    void line_distanceTo()
    {
        Line l(QPointF(0, 0), QPointF(10, 0));
        // Use closestPointTo + FlatGeo::distance to verify distance
        double dist;
        Point closest = l.closestPointTo(QPointF(5, 5), dist);
        QVERIFY(fuzzyEqual(closest.x(), 5.0));
        QVERIFY(fuzzyEqual(closest.y(), 0.0));
        QVERIFY(fuzzyEqual(dist, 5.0));
    }

    void line_closestPointTo()
    {
        Line l(QPointF(0, 0), QPointF(10, 0));
        Point closest = l.closestPointTo(QPointF(5, 3));
        QVERIFY(fuzzyEqual(closest.x(), 5.0));
        QVERIFY(fuzzyEqual(closest.y(), 0.0));
    }

    void line_extendAndShorten()
    {
        Line l(QPointF(0, 0), QPointF(10, 0));
        double origLen = l.length();
        l.extend(5.0);
        QVERIFY(fuzzyEqual(l.length(), origLen + 5.0));
        l.shorten(5.0);
        QVERIFY(fuzzyEqual(l.length(), origLen));
    }

    void line_moveDelta()
    {
        Line l(QPointF(0, 0), QPointF(10, 0));
        l.moveDelta(5.0, 3.0);
        QVERIFY(fuzzyEqual(l.p1().x(), 5.0));
        QVERIFY(fuzzyEqual(l.p1().y(), 3.0));
        QVERIFY(fuzzyEqual(l.p2().x(), 15.0));
        QVERIFY(fuzzyEqual(l.p2().y(), 3.0));
    }

    void line_crossesXAndY()
    {
        Line l(QPointF(0, 0), QPointF(10, 10));
        QVERIFY(l.crossesX(5.0));
        QVERIFY(l.crossesY(5.0));
        QVERIFY(!l.crossesX(15.0));
        QVERIFY(!l.crossesY(15.0));
    }

    void line_equality()
    {
        Line l1(QPointF(1, 2), QPointF(3, 4));
        Line l2(QPointF(1, 2), QPointF(3, 4));
        Line l3(QPointF(5, 6), QPointF(7, 8));
        QVERIFY(l1 == l2);
        QVERIFY(l1 != l3);
    }

    // ---------------------------------------------------------------
    // Rectangle
    // ---------------------------------------------------------------

    void rect_constructFromQRectF()
    {
        QRectF qr(10, 20, 100, 50);
        Rectangle r(qr);
        QVERIFY(fuzzyEqual(r.x(), 10.0));
        QVERIFY(fuzzyEqual(r.y(), 20.0));
        QVERIFY(fuzzyEqual(r.width(), 100.0));
        QVERIFY(fuzzyEqual(r.height(), 50.0));
    }

    void rect_centerPoint()
    {
        Rectangle r(0, 0, 100, 100);
        Point c = r.centerPoint();
        QVERIFY(fuzzyEqual(c.x(), 50.0));
        QVERIFY(fuzzyEqual(c.y(), 50.0));
    }

    void rect_edges()
    {
        Rectangle r(0, 0, 10, 10);
        Line top = r.topEdge();
        Line bottom = r.bottomEdge();
        Line left = r.leftEdge();
        Line right = r.rightEdge();

        // Top edge: y=0, x from 0 to 10
        QVERIFY(fuzzyEqual(top.p1().y(), 0.0));
        QVERIFY(fuzzyEqual(top.p2().y(), 0.0));

        // Bottom edge: y=10
        QVERIFY(fuzzyEqual(bottom.p1().y(), 10.0));
        QVERIFY(fuzzyEqual(bottom.p2().y(), 10.0));

        // Left edge: x=0
        QVERIFY(fuzzyEqual(left.p1().x(), 0.0));
        QVERIFY(fuzzyEqual(left.p2().x(), 0.0));

        // Right edge: x=10
        QVERIFY(fuzzyEqual(right.p1().x(), 10.0));
        QVERIFY(fuzzyEqual(right.p2().x(), 10.0));
    }

    void rect_corners()
    {
        Rectangle r(0, 0, 10, 20);
        Point::List c = r.corners();
        QCOMPARE(c.count(), 4);
    }

    void rect_scale()
    {
        Rectangle r(0, 0, 10, 10);
        Rectangle s = r.scaled(2.0);
        QVERIFY(fuzzyEqual(s.width(), 20.0));
        QVERIFY(fuzzyEqual(s.height(), 20.0));
    }

    void rect_growAndShrink()
    {
        Rectangle r(10, 10, 20, 20);
        Rectangle g = r.grown(5.0);
        // Grow expands each edge outward by 5
        QVERIFY(fuzzyEqual(g.width(), 30.0));
        QVERIFY(fuzzyEqual(g.height(), 30.0));
        QVERIFY(fuzzyEqual(g.x(), 5.0));
        QVERIFY(fuzzyEqual(g.y(), 5.0));

        Rectangle s = r.shrunk(5.0);
        QVERIFY(fuzzyEqual(s.width(), 10.0));
        QVERIFY(fuzzyEqual(s.height(), 10.0));
        QVERIFY(fuzzyEqual(s.x(), 15.0));
        QVERIFY(fuzzyEqual(s.y(), 15.0));
    }

    void rect_fromPoints()
    {
        // fromPoints requires axis-aligned rectangle corners
        Point::List pts;
        pts.append(Point(5, 5));
        pts.append(Point(15, 5));
        pts.append(Point(15, 25));
        pts.append(Point(5, 25));
        Rectangle r = Rectangle::fromPoints(pts);
        QVERIFY(fuzzyEqual(r.x(), 5.0));
        QVERIFY(fuzzyEqual(r.y(), 5.0));
        QVERIFY(fuzzyEqual(r.width(), 10.0));
        QVERIFY(fuzzyEqual(r.height(), 20.0));
    }

    void rect_fromCenterPoint()
    {
        Rectangle r = Rectangle::fromCenterPoint(Point(50, 50), 10);
        QVERIFY(fuzzyEqual(r.x(), 40.0));
        QVERIFY(fuzzyEqual(r.y(), 40.0));
        QVERIFY(fuzzyEqual(r.width(), 20.0));
        QVERIFY(fuzzyEqual(r.height(), 20.0));
    }

    // ---------------------------------------------------------------
    // Circle
    // ---------------------------------------------------------------

    void circle_defaultZero()
    {
        Circle c;
        QVERIFY(fuzzyEqual(c.radius(), 0.0));
    }

    void circle_constructCenterRadius()
    {
        Circle c(QPointF(5, 5), 10.0);
        QVERIFY(fuzzyEqual(c.center().x(), 5.0));
        QVERIFY(fuzzyEqual(c.center().y(), 5.0));
        QVERIFY(fuzzyEqual(c.radius(), 10.0));
    }

    void circle_area()
    {
        Circle c(QPointF(0, 0), 5.0);
        double expected = M_PI * 25.0;
        QVERIFY(fuzzyEqual(c.area(), expected));
    }

    void circle_circumference()
    {
        Circle c(QPointF(0, 0), 5.0);
        double expected = 2.0 * M_PI * 5.0;
        QVERIFY(fuzzyEqual(c.circumference(), expected));
    }

    void circle_diameter()
    {
        Circle c(QPointF(0, 0), 7.0);
        QVERIFY(fuzzyEqual(c.diameter(), 14.0));
    }

    void circle_lineIntersection()
    {
        Circle c(QPointF(0, 0), 5.0);
        // Horizontal line through center
        Line l(QPointF(-10, 0), QPointF(10, 0));
        QVERIFY(c.intersects(l));

        QPointF i1, i2;
        int count = c.intersects(l, i1, i2);
        QCOMPARE(count, 2);
        // Intersection points at (-5,0) and (5,0) in some order
        double xs = qMin(i1.x(), i2.x());
        double xl = qMax(i1.x(), i2.x());
        QVERIFY(fuzzyEqual(xs, -5.0));
        QVERIFY(fuzzyEqual(xl, 5.0));
    }

    void circle_containmentViaFlatGeo()
    {
        Circle c(QPointF(0, 0), 10.0);
        QVERIFY(FlatGeo::isInCircle(QPointF(3, 4), c));    // distance 5 < 10
        QVERIFY(!FlatGeo::isInCircle(QPointF(10, 10), c)); // distance ~14.14 > 10
    }

    // ---------------------------------------------------------------
    // Polygon
    // ---------------------------------------------------------------

    void polygon_invalidWhenEmpty()
    {
        Polygon p;
        QVERIFY(!p.isValid());
    }

    void polygon_validWithThreeVertices()
    {
        Polygon p;
        p.appendPoint(Point(0, 0));
        p.appendPoint(Point(10, 0));
        p.appendPoint(Point(5, 10));
        QVERIFY(p.isValid());
    }

    void polygon_areaSquare()
    {
        // CCW square: 10x10 = area 100
        Polygon p;
        p.appendPoint(Point(0, 0));
        p.appendPoint(Point(10, 0));
        p.appendPoint(Point(10, 10));
        p.appendPoint(Point(0, 10));
        double a = p.area();
        QVERIFY(fuzzyEqual(qAbs(a), 100.0));
    }

    void polygon_areaTriangle()
    {
        // Triangle with base 10, height 10 => area 50
        Polygon p;
        p.appendPoint(Point(0, 0));
        p.appendPoint(Point(10, 0));
        p.appendPoint(Point(5, 10));
        double a = p.area();
        QVERIFY(fuzzyEqual(qAbs(a), 50.0));
    }

    void polygon_centroidSquare()
    {
        Polygon p;
        p.appendPoint(Point(0, 0));
        p.appendPoint(Point(10, 0));
        p.appendPoint(Point(10, 10));
        p.appendPoint(Point(0, 10));
        Point c = p.centroid();
        QVERIFY(fuzzyEqual(c.x(), 5.0));
        QVERIFY(fuzzyEqual(c.y(), 5.0));
    }

    void polygon_containsPoint()
    {
        Polygon p;
        p.appendPoint(Point(0, 0));
        p.appendPoint(Point(20, 0));
        p.appendPoint(Point(20, 20));
        p.appendPoint(Point(0, 20));
        QVERIFY(p.contains(Point(10, 10)));
        QVERIFY(!p.contains(Point(30, 30)));
    }

    void polygon_vertexCount()
    {
        Polygon p;
        p.appendPoint(Point(0, 0));
        p.appendPoint(Point(5, 0));
        p.appendPoint(Point(5, 5));
        p.appendPoint(Point(0, 5));
        Line::List edges = p.lines();
        // A 4-vertex polygon has 4 edges (closing edge included)
        QCOMPARE(edges.count(), 4);
    }

    void polygon_boundingRectangle()
    {
        Polygon p;
        p.appendPoint(Point(2, 3));
        p.appendPoint(Point(12, 3));
        p.appendPoint(Point(12, 8));
        p.appendPoint(Point(2, 8));
        Rectangle br = p.boundingRectangle();
        QVERIFY(fuzzyEqual(br.x(), 2.0));
        QVERIFY(fuzzyEqual(br.y(), 3.0));
        QVERIFY(fuzzyEqual(br.width(), 10.0));
        QVERIFY(fuzzyEqual(br.height(), 5.0));
    }

    // ---------------------------------------------------------------
    // Triangle
    // ---------------------------------------------------------------

    void triangle_construction()
    {
        // Triangle class is minimal -- just verify construction does not crash
        Triangle t(QPointF(0, 0), QPointF(10, 0), QPointF(5, 10));
        Q_UNUSED(t);
    }

    // ---------------------------------------------------------------
    // FlatGeo
    // ---------------------------------------------------------------

    void flatGeo_distance()
    {
        double d = FlatGeo::distance(QPointF(0, 0), QPointF(3, 4));
        QVERIFY(fuzzyEqual(d, 5.0));
    }

    void flatGeo_degreesAndRadians()
    {
        double deg = FlatGeo::degrees(M_PI);
        QVERIFY(fuzzyEqual(deg, 180.0));

        double rad = FlatGeo::radians(180.0);
        QVERIFY(fuzzyEqual(rad, M_PI));
    }

    void flatGeo_getPoint()
    {
        // Bearing 90 = east (right), distance 10 => x+10
        Point result = FlatGeo::getPoint(QPointF(0, 0), 90.0, 10.0);
        QVERIFY(fuzzyEqual(result.x(), 10.0));
        QVERIFY(fuzzyEqual(result.y(), 0.0, 0.01));
    }

    void flatGeo_arePointsEqual()
    {
        QVERIFY(FlatGeo::arePointsEqual(QPointF(1.0, 2.0), QPointF(1.0, 2.0)));
        QVERIFY(!FlatGeo::arePointsEqual(QPointF(1.0, 2.0), QPointF(1.5, 2.0)));
    }

    void flatGeo_rotate()
    {
        // Rotate (10,0) around origin by 90 degrees clockwise => (0,10) in screen coords
        QPointF result = FlatGeo::rotate(QPointF(10, 0), QPointF(0, 0), 90.0);
        QVERIFY(fuzzyEqual(result.x(), 0.0, 0.01));
        QVERIFY(fuzzyEqual(result.y(), 10.0, 0.01));
    }

    void flatGeo_move()
    {
        // Move (0,0) bearing 0 (north/up) by 10 => y decreases
        QPointF result = FlatGeo::move(QPointF(0, 0), 0.0, 10.0);
        QVERIFY(fuzzyEqual(result.x(), 0.0, 0.01));
        QVERIFY(fuzzyEqual(result.y(), -10.0, 0.01));
    }

    void flatGeo_isPointSpatialHelpers()
    {
        QPointF a(1, 1);
        QPointF b(5, 5);
        QVERIFY(FlatGeo::isPointLeftOf(a, b));
        QVERIFY(FlatGeo::isPointAbove(a, b));
        QVERIFY(!FlatGeo::isPointRightOf(a, b));
        QVERIFY(!FlatGeo::isPointBelow(a, b));
    }

    void flatGeo_isRectSpatialHelpers()
    {
        QRectF r(0, 0, 10, 10);
        QPointF outside(20, 20);
        QVERIFY(FlatGeo::isRectLeftOf(r, outside));
        QVERIFY(FlatGeo::isRectAbove(r, outside));
    }

    void flatGeo_makePointString()
    {
        QString s = FlatGeo::makePointString(QPointF(3.5, 7.0));
        QVERIFY(s.contains("3.5"));
        QVERIFY(s.contains("7"));
    }

    // ---------------------------------------------------------------
    // Geo namespace helpers
    // ---------------------------------------------------------------

    void geo_directionToBearing()
    {
        QVERIFY(fuzzyEqual(Geo::directionToBearing(Geo::Up), 0.0));
        QVERIFY(fuzzyEqual(Geo::directionToBearing(Geo::ToRight), 90.0));
        QVERIFY(fuzzyEqual(Geo::directionToBearing(Geo::Down), 180.0));
        QVERIFY(fuzzyEqual(Geo::directionToBearing(Geo::ToLeft), 270.0));
    }

    void geo_oppositeDirection()
    {
        QCOMPARE(Geo::oppositeDirection(Geo::Up), Geo::Down);
        QCOMPARE(Geo::oppositeDirection(Geo::ToLeft), Geo::ToRight);
    }

    void geo_bearingToDirection()
    {
        QCOMPARE(Geo::bearingToDirection(0.0), Geo::Up);
        QCOMPARE(Geo::bearingToDirection(90.0), Geo::ToRight);
        QCOMPARE(Geo::bearingToDirection(180.0), Geo::Down);
        QCOMPARE(Geo::bearingToDirection(270.0), Geo::ToLeft);
    }
};

QTEST_MAIN(TstGeometry)
#include "tst_geometry.moc"
