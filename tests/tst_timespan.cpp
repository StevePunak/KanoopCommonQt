#include <QTest>
#include <QDateTime>
#include <Kanoop/timespan.h>
#include <Kanoop/timespanrange.h>

class TstTimeSpan : public QObject
{
    Q_OBJECT

private slots:

    // ========== Construction ==========

    void defaultConstructor()
    {
        TimeSpan ts;
        QVERIFY(ts.isZero());
        QVERIFY(ts.isValid());
        QCOMPARE(ts.totalNanoseconds(), 0.0);
    }

    void componentConstructor()
    {
        // 1 day, 2 hours, 3 minutes, 4 seconds, 500 ms
        TimeSpan ts(1, 2, 3, 4, 500);
        QCOMPARE(ts.days(), 1LL);
        QCOMPARE(ts.hours(), 2LL);
        QCOMPARE(ts.minutes(), 3LL);
        QCOMPARE(ts.seconds(), 4LL);
        QCOMPARE(ts.milliseconds(), 500LL);
    }

    void copyConstructor()
    {
        TimeSpan original = TimeSpan::fromSeconds(42);
        TimeSpan copy(original);
        QCOMPARE(copy.totalSeconds(), 42.0);
        QCOMPARE(copy, original);
    }

    void timespecConstructor()
    {
        struct timespec spec;
        spec.tv_sec = 5;
        spec.tv_nsec = 500000000L; // 0.5 seconds
        TimeSpan ts(spec);
        QCOMPARE(ts.totalSeconds(), 5.5);
    }

    // ========== Factory Methods ==========

    void fromNanoseconds()
    {
        TimeSpan ts = TimeSpan::fromNanoseconds(1500000000.0);
        QCOMPARE(ts.totalSeconds(), 1.5);
    }

    void fromMicroseconds()
    {
        TimeSpan ts = TimeSpan::fromMicroseconds(2500000.0);
        QCOMPARE(ts.totalSeconds(), 2.5);
    }

    void fromMilliseconds()
    {
        TimeSpan ts = TimeSpan::fromMilliseconds(3500.0);
        QCOMPARE(ts.totalSeconds(), 3.5);
    }

    void fromSeconds()
    {
        TimeSpan ts = TimeSpan::fromSeconds(90.0);
        QCOMPARE(ts.totalMinutes(), 1.5);
    }

    void fromMinutes()
    {
        TimeSpan ts = TimeSpan::fromMinutes(90.0);
        QCOMPARE(ts.totalHours(), 1.5);
    }

    void fromHours()
    {
        TimeSpan ts = TimeSpan::fromHours(36.0);
        QCOMPARE(ts.totalDays(), 1.5);
    }

    void fromDays()
    {
        TimeSpan ts = TimeSpan::fromDays(2.5);
        QCOMPARE(ts.totalHours(), 60.0);
    }

    void zeroFactory()
    {
        TimeSpan ts = TimeSpan::zero();
        QVERIFY(ts.isZero());
        QVERIFY(ts.isValid());
    }

    void invalidFactory()
    {
        TimeSpan ts = TimeSpan::invalid();
        QVERIFY(!ts.isValid());
    }

    // ========== Field Accessors ==========

    void fieldAccessorsComplex()
    {
        // 3 days, 14 hours, 25 minutes, 36 seconds, 789 ms
        TimeSpan ts(3, 14, 25, 36, 789);
        QCOMPARE(ts.days(), 3LL);
        QCOMPARE(ts.hours(), 14LL);
        QCOMPARE(ts.minutes(), 25LL);
        QCOMPARE(ts.seconds(), 36LL);
        QCOMPARE(ts.milliseconds(), 789LL);
    }

    void fieldAccessorsOverflow()
    {
        // 25 hours should report as 1 day + 1 hour
        TimeSpan ts = TimeSpan::fromHours(25);
        QCOMPARE(ts.days(), 1LL);
        QCOMPARE(ts.hours(), 1LL);
        QCOMPARE(ts.minutes(), 0LL);
        QCOMPARE(ts.seconds(), 0LL);
    }

    // ========== Total Accessors ==========

    void totalAccessors()
    {
        TimeSpan ts = TimeSpan::fromDays(1);
        QCOMPARE(ts.totalDays(), 1.0);
        QCOMPARE(ts.totalHours(), 24.0);
        QCOMPARE(ts.totalMinutes(), 1440.0);
        QCOMPARE(ts.totalSeconds(), 86400.0);
        QCOMPARE(ts.totalMilliseconds(), 86400000.0);
        QCOMPARE(ts.totalMicroseconds(), 86400000000.0);
        QCOMPARE(ts.totalNanoseconds(), 86400000000000.0);
    }

    // ========== Boolean Queries ==========

    void isZero_isPositive_isNegative()
    {
        TimeSpan zero = TimeSpan::zero();
        QVERIFY(zero.isZero());
        QVERIFY(!zero.isPositive());
        QVERIFY(!zero.isNegative());

        TimeSpan pos = TimeSpan::fromSeconds(1);
        QVERIFY(!pos.isZero());
        QVERIFY(pos.isPositive());
        QVERIFY(!pos.isNegative());

        TimeSpan neg = TimeSpan::fromSeconds(-1);
        QVERIFY(!neg.isZero());
        QVERIFY(!neg.isPositive());
        QVERIFY(neg.isNegative());
    }

    void absoluteValue()
    {
        TimeSpan neg = TimeSpan::fromSeconds(-42);
        TimeSpan abs = neg.absoluteValue();
        QCOMPARE(abs.totalSeconds(), 42.0);
        QVERIFY(abs.isPositive());

        // absoluteValue of positive should stay the same
        TimeSpan pos = TimeSpan::fromSeconds(7);
        QCOMPARE(pos.absoluteValue().totalSeconds(), 7.0);
    }

    // ========== Arithmetic Operators ==========

    void additionOperator()
    {
        TimeSpan a = TimeSpan::fromSeconds(10);
        TimeSpan b = TimeSpan::fromSeconds(20);
        TimeSpan c = a + b;
        QCOMPARE(c.totalSeconds(), 30.0);
    }

    void subtractionOperator()
    {
        TimeSpan a = TimeSpan::fromSeconds(30);
        TimeSpan b = TimeSpan::fromSeconds(10);
        TimeSpan c = a - b;
        QCOMPARE(c.totalSeconds(), 20.0);

        // Subtraction producing negative
        TimeSpan d = b - a;
        QCOMPARE(d.totalSeconds(), -20.0);
        QVERIFY(d.isNegative());
    }

    void multiplyByScalar()
    {
        TimeSpan ts = TimeSpan::fromSeconds(10);
        TimeSpan doubled = ts * 2.0;
        QCOMPARE(doubled.totalSeconds(), 20.0);

        TimeSpan halved = ts * 0.5;
        QCOMPARE(halved.totalSeconds(), 5.0);
    }

    void divideByScalar()
    {
        TimeSpan ts = TimeSpan::fromSeconds(10);
        TimeSpan halved = ts / 2.0;
        QCOMPARE(halved.totalSeconds(), 5.0);
    }

    void compoundAssignmentOperators()
    {
        TimeSpan ts = TimeSpan::fromSeconds(10);

        ts += TimeSpan::fromSeconds(5);
        QCOMPARE(ts.totalSeconds(), 15.0);

        ts -= TimeSpan::fromSeconds(3);
        QCOMPARE(ts.totalSeconds(), 12.0);

        ts *= 2.0;
        QCOMPARE(ts.totalSeconds(), 24.0);

        ts /= 4.0;
        QCOMPARE(ts.totalSeconds(), 6.0);
    }

    // ========== Comparison Operators ==========

    void equalityOperator()
    {
        TimeSpan a = TimeSpan::fromSeconds(5);
        TimeSpan b = TimeSpan::fromMilliseconds(5000);
        QVERIFY(a == b);
        QVERIFY(!(a != b));
    }

    void inequalityOperator()
    {
        TimeSpan a = TimeSpan::fromSeconds(5);
        TimeSpan b = TimeSpan::fromSeconds(6);
        QVERIFY(a != b);
        QVERIFY(!(a == b));
    }

    void lessThanGreaterThan()
    {
        TimeSpan small = TimeSpan::fromSeconds(1);
        TimeSpan big = TimeSpan::fromSeconds(10);

        QVERIFY(small < big);
        QVERIFY(big > small);
        QVERIFY(!(big < small));
        QVERIFY(!(small > big));
    }

    void lessEqualGreaterEqual()
    {
        TimeSpan a = TimeSpan::fromSeconds(5);
        TimeSpan b = TimeSpan::fromSeconds(5);
        TimeSpan c = TimeSpan::fromSeconds(10);

        QVERIFY(a <= b);
        QVERIFY(a >= b);
        QVERIFY(a <= c);
        QVERIFY(c >= a);
        QVERIFY(!(c <= a));
        QVERIFY(!(a >= c));
    }

    // ========== Add Methods ==========

    void addMethods()
    {
        TimeSpan base = TimeSpan::fromSeconds(60);

        QCOMPARE(base.addNanoseconds(1000000000.0).totalSeconds(), 61.0);
        QCOMPARE(base.addMicroseconds(1000000.0).totalSeconds(), 61.0);
        QCOMPARE(base.addMilliseconds(1000.0).totalSeconds(), 61.0);
        QCOMPARE(base.addSeconds(1.0).totalSeconds(), 61.0);
        QCOMPARE(base.addMinutes(1.0).totalSeconds(), 120.0);
        QCOMPARE(base.addHours(1.0).totalMinutes(), 61.0);
        QCOMPARE(base.addDays(1.0).totalHours(), 24.0 + 1.0 / 60.0);

        // Verify original is not mutated
        QCOMPARE(base.totalSeconds(), 60.0);
    }

    // ========== Min / Max ==========

    void minMaxStatic()
    {
        TimeSpan a = TimeSpan::fromSeconds(5);
        TimeSpan b = TimeSpan::fromSeconds(10);

        QCOMPARE(TimeSpan::max(a, b), b);
        QCOMPARE(TimeSpan::min(a, b), a);
        QCOMPARE(TimeSpan::max(b, a), b);
        QCOMPARE(TimeSpan::min(b, a), a);
    }

    // ========== diff / absDiff ==========

    void diffQDateTime()
    {
        QDateTime now = QDateTime::currentDateTimeUtc();
        QDateTime later = now.addSecs(120);

        TimeSpan d = TimeSpan::diff(now, later);
        QCOMPARE(d.totalSeconds(), 120.0);

        // diff(now, then) = then - now, so diff(later, now) is negative
        TimeSpan d2 = TimeSpan::diff(later, now);
        QCOMPARE(d2.totalSeconds(), -120.0);
    }

    void absDiffQDateTime()
    {
        QDateTime now = QDateTime::currentDateTimeUtc();
        QDateTime later = now.addSecs(120);

        TimeSpan d1 = TimeSpan::absDiff(now, later);
        TimeSpan d2 = TimeSpan::absDiff(later, now);
        QCOMPARE(d1.totalSeconds(), 120.0);
        QCOMPARE(d2.totalSeconds(), 120.0);
    }

    // ========== toTimeSpec ==========

    void toTimeSpecConversion()
    {
        TimeSpan ts = TimeSpan::fromSeconds(3.5);
        struct timespec spec;
        ts.toTimeSpec(spec);
        QCOMPARE(spec.tv_sec, 3L);
        QCOMPARE(spec.tv_nsec, 500000000L);
    }

    // ========== QVariant Round-Trip ==========

    void variantRoundTrip()
    {
        TimeSpan original = TimeSpan::fromSeconds(99);
        QVariant v = original.toVariant();
        TimeSpan restored = TimeSpan::fromVariant(v);
        QCOMPARE(restored, original);
    }

    // ========== Parsing: fromString ==========

    void fromStringColonDelimited()
    {
        bool parsed = false;

        // HH:MM:SS
        TimeSpan ts1 = TimeSpan::fromString("01:30:00", &parsed);
        QVERIFY(parsed);
        QCOMPARE(ts1.hours(), 1LL);
        QCOMPARE(ts1.minutes(), 30LL);
        QCOMPARE(ts1.seconds(), 0LL);

        // HH:MM:SS.mmm
        TimeSpan ts2 = TimeSpan::fromString("00:00:01.500", &parsed);
        QVERIFY(parsed);
        QCOMPARE(ts2.seconds(), 1LL);
        QCOMPARE(ts2.milliseconds(), 500LL);

        // DD:HH:MM:SS
        TimeSpan ts3 = TimeSpan::fromString("2:12:00:00", &parsed);
        QVERIFY(parsed);
        QCOMPARE(ts3.days(), 2LL);
        QCOMPARE(ts3.hours(), 12LL);
    }

    void fromStringAbbreviated()
    {
        bool parsed = false;

        TimeSpan ts1 = TimeSpan::fromString("1d 2h 3m 4s", &parsed);
        QVERIFY(parsed);
        QCOMPARE(ts1.days(), 1LL);
        QCOMPARE(ts1.hours(), 2LL);
        QCOMPARE(ts1.minutes(), 3LL);
        QCOMPARE(ts1.seconds(), 4LL);

        // Just hours and minutes
        TimeSpan ts2 = TimeSpan::fromString("5h 30m", &parsed);
        QVERIFY(parsed);
        QCOMPARE(ts2.hours(), 5LL);
        QCOMPARE(ts2.minutes(), 30LL);

        // Just seconds
        TimeSpan ts3 = TimeSpan::fromString("45s", &parsed);
        QVERIFY(parsed);
        QCOMPARE(ts3.totalSeconds(), 45.0);
    }

    void fromStringMilliseconds()
    {
        bool parsed = false;

        TimeSpan ts = TimeSpan::fromString("1500ms", &parsed);
        QVERIFY(parsed);
        QCOMPARE(ts.totalMilliseconds(), 1500.0);
    }

    void fromStringMicroseconds()
    {
        bool parsed = false;

        TimeSpan ts = TimeSpan::fromString("2500us", &parsed);
        QVERIFY(parsed);
        QCOMPARE(ts.totalMicroseconds(), 2500.0);
    }

    void fromStringPlainNumber()
    {
        // A plain number should be interpreted as milliseconds
        bool parsed = false;
        TimeSpan ts = TimeSpan::fromString("5000", &parsed);
        QVERIFY(parsed);
        QCOMPARE(ts.totalMilliseconds(), 5000.0);
    }

    void fromStringEmpty()
    {
        // Empty string should return zero
        TimeSpan ts = TimeSpan::fromString("");
        QVERIFY(ts.isZero());
    }

    // ========== toString ==========

    void toStringAutoFormat()
    {
        // A span >= 1ms uses the colon-delimited format
        TimeSpan ts(0, 1, 30, 45, 123);
        QString str = ts.toString();
        QVERIFY2(str.contains(":"), qPrintable("Expected colon-delimited format, got: " + str));
    }

    void toStringAbbreviatedFormat()
    {
        TimeSpan ts(1, 2, 3, 4);
        QString abbr = ts.toAbbreviatedFormat();
        QVERIFY2(abbr.contains("d"), qPrintable("Expected 'd' in abbreviated: " + abbr));
        QVERIFY2(abbr.contains("h"), qPrintable("Expected 'h' in abbreviated: " + abbr));
        QVERIFY2(abbr.contains("m"), qPrintable("Expected 'm' in abbreviated: " + abbr));
        QVERIFY2(abbr.contains("s"), qPrintable("Expected 's' in abbreviated: " + abbr));
    }

    void toStringZeroAbbreviated()
    {
        TimeSpan zero = TimeSpan::zero();
        QCOMPARE(zero.toAbbreviatedFormat(), QString("0m"));
    }

    void toStringNegativeAbbreviated()
    {
        TimeSpan neg = TimeSpan::fromHours(-2);
        QString str = neg.toAbbreviatedFormat();
        QVERIFY2(str.startsWith('-'), qPrintable("Expected '-' prefix, got: " + str));
    }

    // ========== Edge Cases ==========

    void negativeConstruction()
    {
        TimeSpan ts = TimeSpan::fromSeconds(-3600);
        QVERIFY(ts.isNegative());
        QCOMPARE(ts.totalHours(), -1.0);
        QCOMPARE(ts.absoluteValue().totalHours(), 1.0);
    }

    void veryLargeValue()
    {
        // 100 years worth of days (approximately)
        TimeSpan ts = TimeSpan::fromDays(36525);
        QVERIFY(ts.isPositive());
        QVERIFY(ts.isValid());
        double years = ts.totalDays() / 365.25;
        QVERIFY(qAbs(years - 100.0) < 0.01);
    }

    void subMicrosecondPrecision()
    {
        TimeSpan ts = TimeSpan::fromNanoseconds(500);
        QCOMPARE(ts.totalNanoseconds(), 500.0);
        QVERIFY(!ts.isZero());
    }

    // ========== TimeSpanRange ==========

    void rangeConstruction()
    {
        TimeSpan start = TimeSpan::fromSeconds(10);
        TimeSpan end = TimeSpan::fromSeconds(20);
        TimeSpanRange range(start, end);

        QCOMPARE(range.startOffset(), start);
        QCOMPARE(range.endOffset(), end);
        QVERIFY(range.isValid());
    }

    void rangeDuration()
    {
        TimeSpanRange range(TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(30));
        QCOMPARE(range.duration().totalSeconds(), 20.0);
    }

    void rangeMidpoint()
    {
        TimeSpanRange range(TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(30));
        QCOMPARE(range.midpoint().totalSeconds(), 20.0);
    }

    void rangeContainsTimeSpan()
    {
        TimeSpanRange range(TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(20));

        QVERIFY(range.contains(TimeSpan::fromSeconds(15)));   // inside
        QVERIFY(range.contains(TimeSpan::fromSeconds(10)));   // start boundary (inclusive)
        QVERIFY(range.contains(TimeSpan::fromSeconds(20)));   // end boundary (inclusive)
        QVERIFY(!range.contains(TimeSpan::fromSeconds(5)));   // before
        QVERIFY(!range.contains(TimeSpan::fromSeconds(25)));  // after
    }

    void rangeContainsRange()
    {
        TimeSpanRange outer(TimeSpan::fromSeconds(0), TimeSpan::fromSeconds(100));
        TimeSpanRange inner(TimeSpan::fromSeconds(20), TimeSpan::fromSeconds(80));
        TimeSpanRange partial(TimeSpan::fromSeconds(50), TimeSpan::fromSeconds(150));

        QVERIFY(outer.contains(inner));
        QVERIFY(!outer.contains(partial));
        QVERIFY(!inner.contains(outer));
    }

    void rangeFromMidpoint()
    {
        TimeSpanRange range = TimeSpanRange::fromMidpoint(
            TimeSpan::fromSeconds(50), TimeSpan::fromSeconds(20));
        QCOMPARE(range.startOffset().totalSeconds(), 40.0);
        QCOMPARE(range.endOffset().totalSeconds(), 60.0);
        QCOMPARE(range.midpoint().totalSeconds(), 50.0);
    }

    void rangeFromStartAndDuration()
    {
        TimeSpanRange range = TimeSpanRange::fromStartAndDuration(
            TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(30));
        QCOMPARE(range.startOffset().totalSeconds(), 10.0);
        QCOMPARE(range.endOffset().totalSeconds(), 40.0);
        QCOMPARE(range.duration().totalSeconds(), 30.0);
    }

    void rangeSetters()
    {
        TimeSpanRange range(TimeSpan::fromSeconds(0), TimeSpan::fromSeconds(10));
        range.setStartOffset(TimeSpan::fromSeconds(5));
        range.setEndOffset(TimeSpan::fromSeconds(15));
        QCOMPARE(range.startOffset().totalSeconds(), 5.0);
        QCOMPARE(range.endOffset().totalSeconds(), 15.0);
    }

    void rangeAddSubtract()
    {
        TimeSpanRange range(TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(20));
        TimeSpan shift = TimeSpan::fromSeconds(5);

        TimeSpanRange shifted = range + shift;
        QCOMPARE(shifted.startOffset().totalSeconds(), 15.0);
        QCOMPARE(shifted.endOffset().totalSeconds(), 25.0);

        TimeSpanRange unshifted = shifted - shift;
        QCOMPARE(unshifted.startOffset().totalSeconds(), 10.0);
        QCOMPARE(unshifted.endOffset().totalSeconds(), 20.0);
    }

    void rangeCompoundAddAssign()
    {
        TimeSpanRange range(TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(20));
        range += TimeSpan::fromSeconds(5);
        QCOMPARE(range.startOffset().totalSeconds(), 15.0);
        QCOMPARE(range.endOffset().totalSeconds(), 25.0);
    }

    void rangeCompoundSubtractAssign()
    {
        TimeSpanRange range(TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(20));
        range -= TimeSpan::fromSeconds(3);
        QCOMPARE(range.startOffset().totalSeconds(), 7.0);
        QCOMPARE(range.endOffset().totalSeconds(), 17.0);
    }

    void rangeEquality()
    {
        TimeSpanRange a(TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(20));
        TimeSpanRange b(TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(20));
        TimeSpanRange c(TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(30));

        QVERIFY(a == b);
        QVERIFY(!(a != b));
        QVERIFY(a != c);
    }

    void rangeCopyConstructor()
    {
        TimeSpanRange original(TimeSpan::fromSeconds(5), TimeSpan::fromSeconds(15));
        TimeSpanRange copy(original);
        QCOMPARE(copy, original);
        QCOMPARE(copy.duration().totalSeconds(), 10.0);
    }

    void rangeSubRange()
    {
        TimeSpanRange a(TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(50));
        TimeSpanRange b(TimeSpan::fromSeconds(20), TimeSpan::fromSeconds(40));

        // subRange clamps a to the bounds of b — intersection
        TimeSpanRange intersection = a.subRange(b);
        QCOMPARE(intersection.startOffset().totalSeconds(), 20.0);
        QCOMPARE(intersection.endOffset().totalSeconds(), 40.0);
    }

    void rangeMerged()
    {
        TimeSpanRange a(TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(30));
        TimeSpanRange b(TimeSpan::fromSeconds(20), TimeSpan::fromSeconds(50));

        TimeSpanRange m = a.merged(b);
        QCOMPARE(m.startOffset().totalSeconds(), 10.0);
        QCOMPARE(m.endOffset().totalSeconds(), 50.0);

        // Static version should produce the same result
        TimeSpanRange m2 = TimeSpanRange::merged(a, b);
        QCOMPARE(m2, m);
    }

    void rangeToString()
    {
        TimeSpanRange range(TimeSpan::fromSeconds(10), TimeSpan::fromSeconds(20));
        QString str = range.toString();
        QVERIFY2(!str.isEmpty(), "toString should produce non-empty output");
    }

    void rangeListConsolidated()
    {
        TimeSpanRange::List list;
        list.append(TimeSpanRange(TimeSpan::fromSeconds(0), TimeSpan::fromSeconds(10)));
        list.append(TimeSpanRange(TimeSpan::fromSeconds(5), TimeSpan::fromSeconds(15)));
        list.append(TimeSpanRange(TimeSpan::fromSeconds(20), TimeSpan::fromSeconds(30)));

        TimeSpanRange::List result = list.consolidated();

        // First two overlap -> merged into [0,15], third is separate -> [20,30]
        QCOMPARE(result.size(), 2);
        QCOMPARE(result[0].startOffset().totalSeconds(), 0.0);
        QCOMPARE(result[0].endOffset().totalSeconds(), 15.0);
        QCOMPARE(result[1].startOffset().totalSeconds(), 20.0);
        QCOMPARE(result[1].endOffset().totalSeconds(), 30.0);
    }
};

QTEST_MAIN(TstTimeSpan)
#include "tst_timespan.moc"
