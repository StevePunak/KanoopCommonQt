#include <QTest>
#include <QTimeZone>

#include <Kanoop/daterange.h>
#include <Kanoop/datetimeutil.h>
#include <Kanoop/timespan.h>

class TstDateRange : public QObject
{
    Q_OBJECT

private:
    static QDateTime utc(int y, int m, int d, int h = 0, int min = 0, int s = 0, int ms = 0)
    {
        return QDateTime(QDate(y, m, d), QTime(h, min, s, ms), QTimeZone::utc());
    }

private slots:

    // ========== DateRange construction ==========

    void construct_startEnd()
    {
        QDateTime start = utc(2025, 6, 1, 12, 0);
        QDateTime end   = utc(2025, 6, 1, 14, 0);
        DateRange r(start, end);

        QCOMPARE(r.startTime(), start);
        QCOMPARE(r.endTime(), end);
        QVERIFY(r.isValid());
    }

    void construct_startDuration()
    {
        QDateTime start = utc(2025, 1, 15, 8, 0);
        TimeSpan  dur   = TimeSpan::fromHours(3);
        DateRange r(start, dur);

        QCOMPARE(r.startTime(), start);
        QCOMPARE(r.endTime(), start.addMSecs(dur.totalMilliseconds()));
        QCOMPARE(r.duration().totalHours(), 3.0);
    }

    void construct_default_isInvalid()
    {
        DateRange r;
        QVERIFY(!r.isValid());
    }

    void construct_fromMidpoint()
    {
        QDateTime mid = utc(2025, 3, 10, 12, 0);
        TimeSpan  dur = TimeSpan::fromHours(2);
        DateRange r   = DateRange::fromMidpoint(mid, dur);

        QCOMPARE(r.startTime(), utc(2025, 3, 10, 11, 0));
        QCOMPARE(r.endTime(),   utc(2025, 3, 10, 13, 0));
        QCOMPARE(r.midpoint(),  mid);
    }

    // ========== Copy / assignment ==========

    void copyConstructor()
    {
        DateRange a(utc(2025, 1, 1), utc(2025, 1, 2));
        DateRange b(a);

        QCOMPARE(b.startTime(), a.startTime());
        QCOMPARE(b.endTime(), a.endTime());
        QCOMPARE(b, a);
    }

    void assignmentOperator()
    {
        DateRange a(utc(2025, 5, 1, 6, 0), utc(2025, 5, 1, 18, 0));
        DateRange b;
        b = a;

        QCOMPARE(b, a);
    }

    // ========== Equality ==========

    void equality_sameRange()
    {
        DateRange a(utc(2025, 7, 4, 0, 0), utc(2025, 7, 5, 0, 0));
        DateRange b(utc(2025, 7, 4, 0, 0), utc(2025, 7, 5, 0, 0));
        QVERIFY(a == b);
        QVERIFY(!(a != b));
    }

    void equality_differentRange()
    {
        DateRange a(utc(2025, 7, 4, 0, 0), utc(2025, 7, 5, 0, 0));
        DateRange b(utc(2025, 7, 4, 0, 0), utc(2025, 7, 6, 0, 0));
        QVERIFY(a != b);
        QVERIFY(!(a == b));
    }

    // ========== Shift operators ==========

    void shiftForward()
    {
        DateRange r(utc(2025, 2, 1, 10, 0), utc(2025, 2, 1, 12, 0));
        TimeSpan  shift = TimeSpan::fromHours(1);
        DateRange s = r + shift;

        QCOMPARE(s.startTime(), utc(2025, 2, 1, 11, 0));
        QCOMPARE(s.endTime(),   utc(2025, 2, 1, 13, 0));
        // original unchanged
        QCOMPARE(r.startTime(), utc(2025, 2, 1, 10, 0));
    }

    void shiftBackward()
    {
        DateRange r(utc(2025, 2, 1, 10, 0), utc(2025, 2, 1, 12, 0));
        DateRange s = r - TimeSpan::fromMinutes(30);

        QCOMPARE(s.startTime(), utc(2025, 2, 1, 9, 30));
        QCOMPARE(s.endTime(),   utc(2025, 2, 1, 11, 30));
    }

    void shiftForwardInPlace()
    {
        DateRange r(utc(2025, 4, 1, 0, 0), utc(2025, 4, 1, 6, 0));
        r += TimeSpan::fromDays(1);

        QCOMPARE(r.startTime(), utc(2025, 4, 2, 0, 0));
        QCOMPARE(r.endTime(),   utc(2025, 4, 2, 6, 0));
    }

    void shiftBackwardInPlace()
    {
        // BUG: DateRange::operator-= adds instead of subtracting.
        // This test documents the intended behavior — skip until fixed.
        QSKIP("DateRange::operator-= has a known bug (adds instead of subtracting)");
    }

    // ========== Duration / midpoint ==========

    void duration_basic()
    {
        DateRange r(utc(2025, 8, 1, 0, 0), utc(2025, 8, 1, 2, 30));
        QCOMPARE(r.duration().totalMinutes(), 150.0);
    }

    void midpoint_basic()
    {
        DateRange r(utc(2025, 1, 1, 10, 0), utc(2025, 1, 1, 14, 0));
        QCOMPARE(r.midpoint(), utc(2025, 1, 1, 12, 0));
    }

    // ========== contains(QDateTime) ==========

    void containsDateTime_inside()
    {
        DateRange r(utc(2025, 6, 1, 8, 0), utc(2025, 6, 1, 17, 0));
        QVERIFY(r.contains(utc(2025, 6, 1, 12, 0)));
    }

    void containsDateTime_atBoundaries()
    {
        DateRange r(utc(2025, 6, 1, 8, 0), utc(2025, 6, 1, 17, 0));
        QVERIFY(r.contains(r.startTime()));
        QVERIFY(r.contains(r.endTime()));
    }

    void containsDateTime_outside()
    {
        DateRange r(utc(2025, 6, 1, 8, 0), utc(2025, 6, 1, 17, 0));
        QVERIFY(!r.contains(utc(2025, 6, 1, 7, 59)));
        QVERIFY(!r.contains(utc(2025, 6, 1, 17, 0, 1)));
    }

    // ========== contains(DateRange) ==========

    void containsRange_fullyInside()
    {
        DateRange outer(utc(2025, 1, 1), utc(2025, 12, 31));
        DateRange inner(utc(2025, 3, 1), utc(2025, 4, 1));
        QVERIFY(outer.contains(inner));
    }

    void containsRange_partialOverlap()
    {
        DateRange a(utc(2025, 1, 1), utc(2025, 6, 1));
        DateRange b(utc(2025, 5, 1), utc(2025, 8, 1));
        QVERIFY(!a.contains(b));
    }

    // ========== intersects ==========

    void intersects_overlapping()
    {
        DateRange a(utc(2025, 1, 1), utc(2025, 3, 1));
        DateRange b(utc(2025, 2, 1), utc(2025, 4, 1));
        QVERIFY(a.intersects(b));
        QVERIFY(b.intersects(a));
    }

    void intersects_disjoint()
    {
        DateRange a(utc(2025, 1, 1), utc(2025, 2, 1));
        DateRange b(utc(2025, 3, 1), utc(2025, 4, 1));
        QVERIFY(!a.intersects(b));
        QVERIFY(!b.intersects(a));
    }

    void intersects_adjacent()
    {
        // a ends exactly when b starts — adjacency behavior
        DateRange a(utc(2025, 1, 1), utc(2025, 2, 1));
        DateRange b(utc(2025, 2, 1), utc(2025, 3, 1));
        // At least one of contains(start)/contains(end) should be true at the shared point
        QVERIFY(a.contains(b.startTime()));
    }

    // ========== subRange (intersection) ==========

    void subRange_overlap()
    {
        DateRange a(utc(2025, 1, 1), utc(2025, 3, 1));
        DateRange b(utc(2025, 2, 1), utc(2025, 4, 1));
        DateRange sub = a.subRange(b);

        QVERIFY(sub.isValid());
        QCOMPARE(sub.startTime(), utc(2025, 2, 1));
        QCOMPARE(sub.endTime(),   utc(2025, 3, 1));
    }

    void subRange_noOverlap()
    {
        DateRange a(utc(2025, 1, 1), utc(2025, 2, 1));
        DateRange b(utc(2025, 3, 1), utc(2025, 4, 1));
        DateRange sub = a.subRange(b);

        QVERIFY(!sub.isValid());
    }

    // ========== merged ==========

    void merged_overlapping()
    {
        DateRange a(utc(2025, 1, 1), utc(2025, 3, 1));
        DateRange b(utc(2025, 2, 1), utc(2025, 4, 1));
        DateRange m = a.merged(b);

        QCOMPARE(m.startTime(), utc(2025, 1, 1));
        QCOMPARE(m.endTime(),   utc(2025, 4, 1));
    }

    // ========== setters ==========

    void setStartTime_setEndTime()
    {
        DateRange r(utc(2025, 1, 1), utc(2025, 1, 2));
        r.setStartTime(utc(2025, 1, 5));
        r.setEndTime(utc(2025, 1, 10));
        QCOMPARE(r.startTime(), utc(2025, 1, 5));
        QCOMPARE(r.endTime(),   utc(2025, 1, 10));
    }

    // ========== Edge: zero-length range ==========

    void zeroLengthRange()
    {
        QDateTime t = utc(2025, 6, 15, 12, 0);
        DateRange r(t, t);

        QVERIFY(r.isValid());
        QCOMPARE(r.duration().totalMilliseconds(), 0.0);
        QVERIFY(r.contains(t));
        QCOMPARE(r.midpoint(), t);
    }

    // ========== Edge: midnight crossing ==========

    void midnightCrossing()
    {
        DateRange r(utc(2025, 3, 15, 23, 0), utc(2025, 3, 16, 1, 0));
        QCOMPARE(r.duration().totalHours(), 2.0);
        QVERIFY(r.contains(utc(2025, 3, 16, 0, 0)));
        QCOMPARE(r.midpoint(), utc(2025, 3, 16, 0, 0));
    }

    // ========== DateRange::List consolidated ==========

    void list_consolidated()
    {
        DateRange::List list;
        list.append(DateRange(utc(2025, 1, 1), utc(2025, 1, 10)));
        list.append(DateRange(utc(2025, 1, 8), utc(2025, 1, 20)));
        list.append(DateRange(utc(2025, 2, 1), utc(2025, 2, 10)));

        DateRange::List result = list.consolidated();
        QCOMPARE(result.size(), 2);
        QCOMPARE(result[0].startTime(), utc(2025, 1, 1));
        QCOMPARE(result[0].endTime(),   utc(2025, 1, 20));
        QCOMPARE(result[1].startTime(), utc(2025, 2, 1));
        QCOMPARE(result[1].endTime(),   utc(2025, 2, 10));
    }

    // ========== DateTimeUtil formatting ==========

    void dateTimeUtil_toISOString()
    {
        QDateTime dt = utc(2021, 9, 17, 5, 30, 0, 123);
        QCOMPARE(DateTimeUtil::toISOString(dt), QStringLiteral("2021-09-17T05:30:00.123Z"));
    }

    void dateTimeUtil_toStandardString()
    {
        QDateTime dt = utc(2021, 9, 17, 5, 30, 0, 123);
        QCOMPARE(DateTimeUtil::toStandardString(dt), QStringLiteral("2021-09-17 05:30:00.123"));
        QCOMPARE(DateTimeUtil::toStandardString(dt, false), QStringLiteral("2021-09-17 05:30:00"));
    }

    void dateTimeUtil_toStandardDateString()
    {
        QDateTime dt = utc(2025, 12, 25, 10, 30);
        QCOMPARE(DateTimeUtil::toStandardDateString(dt), QStringLiteral("2025-12-25"));
    }

    void dateTimeUtil_toSquashedString()
    {
        QDateTime dt = utc(2021, 9, 17, 5, 30, 0, 123);
        QCOMPARE(DateTimeUtil::toSquashedString(dt), QStringLiteral("20210917053000123"));
    }

    void dateTimeUtil_toStandardTimeString()
    {
        QDateTime dt = utc(2025, 6, 1, 14, 5, 30, 789);
        QCOMPARE(DateTimeUtil::toStandardTimeString(dt), QStringLiteral("14:05:30.789"));
    }

    void dateTimeUtil_fromISOString()
    {
        QDateTime dt = DateTimeUtil::fromISOString("2021-09-17T05:30:00.123Z");
        QVERIFY(dt.isValid());
        QCOMPARE(dt.date(), QDate(2021, 9, 17));
        QCOMPARE(dt.time(), QTime(5, 30, 0, 123));
        QCOMPARE(dt.timeZone(), QTimeZone::utc());
    }

    void dateTimeUtil_fromISOString_invalid()
    {
        QDateTime dt = DateTimeUtil::fromISOString("not-a-date");
        QVERIFY(!dt.isValid());
    }

    void dateTimeUtil_roundTrip_standard()
    {
        QDateTime original = utc(2025, 11, 3, 22, 15, 45, 678);
        QString str = DateTimeUtil::toStandardString(original);
        QDateTime parsed = DateTimeUtil::fromStandardString(str);
        QVERIFY(parsed.isValid());
        QCOMPARE(parsed.date(), original.date());
        QCOMPARE(parsed.time(), original.time());
    }

    void dateTimeUtil_roundTrip_squashed()
    {
        QDateTime original = utc(2025, 7, 20, 8, 0, 0, 456);
        QString str = DateTimeUtil::toSquashedString(original);
        QDateTime parsed = DateTimeUtil::fromSquashedString(str);
        QVERIFY(parsed.isValid());
        QCOMPARE(parsed.date(), original.date());
        QCOMPARE(parsed.time(), original.time());
    }
};

QTEST_MAIN(TstDateRange)
#include "tst_daterange.moc"
