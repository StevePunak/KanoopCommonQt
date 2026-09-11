// Link gate for public members that were declared but never emitted into the
// library. The test for a missing definition is that a consumer links at all,
// so most of the value here is the existence of this translation unit and its
// calls. It is deliberately separate from the suites that also exercise these
// classes so that a link failure names this file and nothing else.
#include <QTest>
#include <QElapsedTimer>

#include <Kanoop/geometry/angle.h>
#include <Kanoop/geometry/ellipse.h>
#include <Kanoop/geometry/point.h>
#include <Kanoop/threadutil.h>

class TstSymbols : public QObject
{
    Q_OBJECT

private slots:

    void threadUtil_spinSleep()
    {
        QElapsedTimer timer;
        timer.start();
        ThreadUtil::spinSleep(5000);            // 5 ms
        const qint64 elapsed = timer.elapsed();
        QVERIFY2(elapsed >= 4,
                 qPrintable(QString("spinSleep(5000 us) returned after %1 ms").arg(elapsed)));
    }

    void threadUtil_spinSleepZeroReturns()
    {
        ThreadUtil::spinSleep(0);
        QVERIFY(true);
    }

    void threadUtil_currentThreadString()
    {
        QVERIFY(ThreadUtil::currentThreadString().isEmpty() == false);
    }

    // Angle measured from vertical: 0 gives semi-axis B, 90 gives semi-axis A.
    void ellipse_radiusAtAngle()
    {
        Ellipse e(Point(0, 0), 3.0, 4.0);
        QVERIFY(qAbs(e.radiusAtAngle(0.0) - 4.0) < 1e-9);
        QVERIFY(qAbs(e.radiusAtAngle(90.0) - 3.0) < 1e-9);
        // 12 / sqrt(9*0.5 + 16*0.5) = 12 / sqrt(12.5)
        QVERIFY(qAbs(e.radiusAtAngle(45.0) - 3.3941125496954285) < 1e-9);
    }

    void ellipse_toString()
    {
        Ellipse e(Point(1, 2), 3.0, 4.0);
        const QString text = e.toString();
        QVERIFY(text.isEmpty() == false);
        QVERIFY(text.contains("3"));
        QVERIFY(text.contains("4"));
    }

    void angle_inPlaceAdd()
    {
        Angle a(10.0);
        a.add(720.0);
        QVERIFY(qAbs(a.degrees() - 10.0) < 1e-9);
    }

    void angle_inPlaceSubtract()
    {
        Angle a(10.0);
        a.subtract(-400.0);
        QVERIFY(qAbs(a.degrees() - 50.0) < 1e-9);
    }
};

QTEST_MAIN(TstSymbols)
#include "tst_symbols.moc"
