#include <QTest>
#include <QtMath>

#include <Kanoop/geo/geocoordinate.h>
#include <Kanoop/geo/geocircle.h>
#include <Kanoop/geo/earthgeo.h>

// ---------------------------------------------------------------------------
// Well-known reference points
// ---------------------------------------------------------------------------
static GeoCoordinate NYC(40.7128, -74.0060);         // New York City
static GeoCoordinate LONDON(51.5074, -0.1278);       // London
static GeoCoordinate LA(34.0522, -118.2437);         // Los Angeles
static GeoCoordinate TOKYO(35.6762, 139.6503);       // Tokyo
static const GeoCoordinate SYDNEY(-33.8688, 151.2093);     // Sydney
static const GeoCoordinate NORTH_POLE(90.0, 0.0);
static const GeoCoordinate SOUTH_POLE(-90.0, 0.0);
static const GeoCoordinate EQUATOR_PRIME(0.0, 0.0);        // Null Island
static const GeoCoordinate EQUATOR_DATELINE(0.0, 180.0);   // On the antimeridian

// Helper: returns true when |actual - expected| / expected <= toleranceFraction
static bool withinRelativeTolerance(double actual, double expected, double toleranceFraction)
{
    if (qFuzzyIsNull(expected))
        return qAbs(actual) < 1.0; // absolute fallback for zero-expected
    return qAbs(actual - expected) / qAbs(expected) <= toleranceFraction;
}

class TstGeo : public QObject
{
    Q_OBJECT

private slots:

    // ================================================================
    //  GeoCoordinate — construction & accessors
    // ================================================================

    void coordinate_defaultConstruction()
    {
        GeoCoordinate c;
        QCOMPARE(c.latitude(), 0.0);
        QCOMPARE(c.longitude(), 0.0);
        QCOMPARE(c.altitude(), 0.0);
        QVERIFY(!c.isValid());
        QVERIFY(c.isEmpty());
    }

    void coordinate_latLonConstruction()
    {
        GeoCoordinate c(45.0, -90.0);
        QCOMPARE(c.latitude(), 45.0);
        QCOMPARE(c.longitude(), -90.0);
        QCOMPARE(c.altitude(), 0.0);
        QVERIFY(c.isValid());
    }

    void coordinate_latLonAltConstruction()
    {
        GeoCoordinate c(48.8566, 2.3522, 35.0);
        QCOMPARE(c.latitude(), 48.8566);
        QCOMPARE(c.longitude(), 2.3522);
        QCOMPARE(c.altitude(), 35.0);
        QVERIFY(c.isValid());
    }

    void coordinate_setters()
    {
        GeoCoordinate c;
        c.setLatitude(10.0);
        c.setLongitude(20.0);
        c.setAltitude(100.0);
        QCOMPARE(c.latitude(), 10.0);
        QCOMPARE(c.longitude(), 20.0);
        QCOMPARE(c.altitude(), 100.0);
    }

    // ================================================================
    //  Equality
    // ================================================================

    void coordinate_equality_same()
    {
        GeoCoordinate a(40.7128, -74.0060);
        GeoCoordinate b(40.7128, -74.0060);
        QVERIFY(a == b);
        QVERIFY(!(a != b));
    }

    void coordinate_equality_different()
    {
        GeoCoordinate a(40.7128, -74.0060);
        GeoCoordinate b(51.5074, -0.1278);
        QVERIFY(a != b);
        QVERIFY(!(a == b));
    }

    void coordinate_equality_precisionBoundary()
    {
        // GeoCoordinate::operator== does exact double comparison.
        // Verify that identical coordinates are equal and slightly
        // different ones are not.
        GeoCoordinate a(40.7128001, -74.0060001);
        GeoCoordinate b(40.7128001, -74.0060001);
        GeoCoordinate c(40.7128002, -74.0060002);
        QVERIFY(a == b);
        QVERIFY(!(a == c));
    }

    // ================================================================
    //  isValid / isEmpty
    // ================================================================

    void coordinate_isValid_validCoord()
    {
        QVERIFY(NYC.isValid());
        QVERIFY(LONDON.isValid());
        QVERIFY(SYDNEY.isValid());
    }

    void coordinate_isValid_defaultIsInvalid()
    {
        GeoCoordinate c;
        QVERIFY(!c.isValid());
    }

    // ================================================================
    //  distanceTo — known city pairs (Haversine, 1% tolerance)
    // ================================================================

    void distanceTo_nycToLondon()
    {
        // Great-circle distance NYC -> London approx 5570 km
        double dist = NYC.distanceTo(LONDON);
        QVERIFY2(withinRelativeTolerance(dist, 5570000.0, 0.01),
                 qPrintable(QString("NYC->London: %1 m, expected ~5570000 m").arg(dist)));
    }

    void distanceTo_laToTokyo()
    {
        // Great-circle distance LA -> Tokyo approx 8815 km
        double dist = LA.distanceTo(TOKYO);
        QVERIFY2(withinRelativeTolerance(dist, 8815000.0, 0.01),
                 qPrintable(QString("LA->Tokyo: %1 m, expected ~8815000 m").arg(dist)));
    }

    void distanceTo_symmetric()
    {
        // distance(A,B) == distance(B,A)
        double ab = NYC.distanceTo(LONDON);
        double ba = LONDON.distanceTo(NYC);
        QVERIFY(qAbs(ab - ba) < 1.0); // within 1 metre
    }

    void distanceTo_samePoint()
    {
        double dist = NYC.distanceTo(NYC);
        QVERIFY(dist < 1.0); // essentially zero
    }

    void distanceTo_poleToEquator()
    {
        // North pole to equator/prime meridian: ~10018 km (quarter circumference)
        double dist = NORTH_POLE.distanceTo(EQUATOR_PRIME);
        QVERIFY2(withinRelativeTolerance(dist, 10018000.0, 0.01),
                 qPrintable(QString("Pole->Equator: %1 m, expected ~10018000 m").arg(dist)));
    }

    void distanceTo_poleToPole()
    {
        // North pole to south pole: ~20036 km (half circumference)
        double dist = NORTH_POLE.distanceTo(SOUTH_POLE);
        QVERIFY2(withinRelativeTolerance(dist, 20036000.0, 0.01),
                 qPrintable(QString("NP->SP: %1 m, expected ~20036000 m").arg(dist)));
    }

    // ================================================================
    //  azimuthTo — known bearings
    // ================================================================

    void azimuthTo_dueNorth()
    {
        // Same longitude, higher latitude => bearing ~0 (north)
        GeoCoordinate south(40.0, -74.0);
        GeoCoordinate north(50.0, -74.0);
        double az = south.azimuthTo(north);
        QVERIFY2(qAbs(az - 0.0) < 1.0 || qAbs(az - 360.0) < 1.0,
                 qPrintable(QString("Expected ~0/360, got %1").arg(az)));
    }

    void azimuthTo_dueSouth()
    {
        GeoCoordinate north(50.0, -74.0);
        GeoCoordinate south(40.0, -74.0);
        double az = north.azimuthTo(south);
        QVERIFY2(qAbs(az - 180.0) < 1.0,
                 qPrintable(QString("Expected ~180, got %1").arg(az)));
    }

    void azimuthTo_dueEast()
    {
        // Same latitude on equator, east of => bearing ~90
        GeoCoordinate west(0.0, 10.0);
        GeoCoordinate east(0.0, 20.0);
        double az = west.azimuthTo(east);
        QVERIFY2(qAbs(az - 90.0) < 1.0,
                 qPrintable(QString("Expected ~90, got %1").arg(az)));
    }

    void azimuthTo_dueWest()
    {
        GeoCoordinate east(0.0, 20.0);
        GeoCoordinate west(0.0, 10.0);
        double az = east.azimuthTo(west);
        QVERIFY2(qAbs(az - 270.0) < 1.0,
                 qPrintable(QString("Expected ~270, got %1").arg(az)));
    }

    // ================================================================
    //  atDistanceAndAzimuth — round-trip
    // ================================================================

    void atDistanceAndAzimuth_roundTrip()
    {
        // Go 1000 m north, then 1000 m south — should return near origin.
        GeoCoordinate origin(45.0, -90.0);
        GeoCoordinate northPoint = origin.atDistanceAndAzimuth(1000.0, EarthGeo::North);
        GeoCoordinate back = northPoint.atDistanceAndAzimuth(1000.0, EarthGeo::South);

        double drift = origin.distanceTo(back);
        QVERIFY2(drift < 1.0,
                 qPrintable(QString("Round-trip drift: %1 m (expected < 1 m)").arg(drift)));
    }

    void atDistanceAndAzimuth_knownDistance()
    {
        // Travel 5000 m east — measure the distance, should be ~5000 m.
        GeoCoordinate origin(45.0, -90.0);
        GeoCoordinate dest = origin.atDistanceAndAzimuth(5000.0, EarthGeo::East);
        double dist = origin.distanceTo(dest);
        QVERIFY2(withinRelativeTolerance(dist, 5000.0, 0.001),
                 qPrintable(QString("Expected ~5000 m, got %1 m").arg(dist)));
    }

    void atDistanceAndAzimuth_zeroDistance()
    {
        GeoCoordinate origin(45.0, -90.0);
        GeoCoordinate same = origin.atDistanceAndAzimuth(0.0, EarthGeo::NorthEast);
        double dist = origin.distanceTo(same);
        QVERIFY(dist < 1.0);
    }

    // ================================================================
    //  Cardinal direction helpers
    // ================================================================

    void isNorthOf_basic()
    {
        QVERIFY(LONDON.isNorthOf(NYC));   // 51.5 > 40.7
        QVERIFY(!NYC.isNorthOf(LONDON));
    }

    void isSouthOf_basic()
    {
        QVERIFY(SYDNEY.isSouthOf(TOKYO));  // -33.8 < 35.6
        QVERIFY(!TOKYO.isSouthOf(SYDNEY));
    }

    void isEastOf_basic()
    {
        QVERIFY(LONDON.isEastOf(NYC));     // -0.12 > -74.0
        QVERIFY(!NYC.isEastOf(LONDON));
    }

    void isWestOf_basic()
    {
        QVERIFY(NYC.isWestOf(LONDON));
        QVERIFY(!LONDON.isWestOf(NYC));
    }

    // ================================================================
    //  GeoCircle — construction & accessors
    // ================================================================

    void circle_defaultConstruction()
    {
        GeoCircle c;
        QCOMPARE(c.radius(), 0.0);
        QVERIFY(!c.isValid());
    }

    void circle_construction()
    {
        GeoCircle c(NYC, 5000.0);
        QVERIFY(c.center() == NYC);
        QCOMPARE(c.radius(), 5000.0);
        QVERIFY(c.isValid());
    }

    void circle_diameterAndArea()
    {
        GeoCircle c(NYC, 1000.0);
        QCOMPARE(c.diameter(), 2000.0);
        double expectedArea = M_PI * 1000.0 * 1000.0;
        QVERIFY(qAbs(c.area() - expectedArea) < 0.01);
    }

    void circle_setters()
    {
        GeoCircle c;
        c.setCenter(LONDON);
        c.setRadius(500.0);
        QVERIFY(c.center() == LONDON);
        QCOMPARE(c.radius(), 500.0);
        QVERIFY(c.isValid());
    }

    // ================================================================
    //  GeoCircle — containment
    // ================================================================

    void circle_containsCenter()
    {
        GeoCircle c(NYC, 1000.0);
        QVERIFY(c.contains(NYC));
    }

    void circle_containsNearby()
    {
        // Point 500 m north of NYC should be inside a 1000 m circle
        GeoCircle c(NYC, 1000.0);
        GeoCoordinate nearby = NYC.atDistanceAndAzimuth(500.0, EarthGeo::North);
        QVERIFY(c.contains(nearby));
    }

    void circle_doesNotContainFaraway()
    {
        // London is ~5570 km from NYC — well outside a 1 km circle
        GeoCircle c(NYC, 1000.0);
        QVERIFY(!c.contains(LONDON));
    }

    void circle_containsBoundary()
    {
        // Point slightly inside the radius should be contained
        GeoCircle c(NYC, 1000.0);
        GeoCoordinate inside = NYC.atDistanceAndAzimuth(999.0, EarthGeo::East);
        QVERIFY(c.contains(inside));

        // Point slightly outside should not be contained
        GeoCoordinate outside = NYC.atDistanceAndAzimuth(1002.0, EarthGeo::East);
        QVERIFY(!c.contains(outside));
    }

    // ================================================================
    //  GeoCircle — equality
    // ================================================================

    void circle_equality()
    {
        GeoCircle a(NYC, 1000.0);
        GeoCircle b(NYC, 1000.0);
        QVERIFY(a == b);
        QVERIFY(!(a != b));

        GeoCircle c(NYC, 2000.0);
        QVERIFY(a != c);

        GeoCircle d(LONDON, 1000.0);
        QVERIFY(a != d);
    }

    // ================================================================
    //  Edge cases
    // ================================================================

    void coordinate_antimeridian_distance()
    {
        // Two points straddling the antimeridian
        GeoCoordinate west(0.0, 179.0);
        GeoCoordinate east(0.0, -179.0);
        double dist = west.distanceTo(east);
        // Should be ~222 km (2 degrees of longitude at the equator)
        QVERIFY2(withinRelativeTolerance(dist, 222000.0, 0.02),
                 qPrintable(QString("Antimeridian distance: %1 m, expected ~222000 m").arg(dist)));
    }

    void coordinate_toString_roundTrip()
    {
        GeoCoordinate original(40.7128, -74.0060, 10.0);
        QString str = original.toString(GeoCoordinate::Parsable);
        GeoCoordinate parsed = GeoCoordinate::fromString(str);
        // Verify they are close (within precision)
        double dist = original.distanceTo(parsed);
        QVERIFY2(dist < 1.0,
                 qPrintable(QString("toString round-trip drift: %1 m").arg(dist)));
    }
};

QTEST_MAIN(TstGeo)
#include "tst_geo.moc"
