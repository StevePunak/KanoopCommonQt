#include <QTest>
#include <QDir>
#include <QFile>
#include <QElapsedTimer>
#include <QThread>
#include <QPointer>

#include <Kanoop/utility/abstractthreadclass.h>


/**
 * @brief Records the order and thread affinity of every lifecycle callback.
 *
 * Allocates a worker-affine QObject in threadStarted() and posts deleteLater()
 * on it from threadAboutToFinish() — verifies the event loop processes the
 * DeferredDelete event before threadFinished() runs.
 */
class LifecycleProbe : public AbstractThreadClass
{
    Q_OBJECT
public:
    LifecycleProbe() : AbstractThreadClass("lifecycle-probe") {}

    QStringList callbackOrder;
    Qt::HANDLE startedTid = nullptr;
    Qt::HANDLE aboutToFinishTid = nullptr;
    Qt::HANDLE finishedTid = nullptr;
    QPointer<QObject> workerChild;
    bool childAliveAtFinished = true;

protected:
    void threadStarted() override
    {
        startedTid = QThread::currentThreadId();
        callbackOrder << "started";
        workerChild = new QObject;
    }

    void threadAboutToFinish() override
    {
        aboutToFinishTid = QThread::currentThreadId();
        callbackOrder << "aboutToFinish";
        if(workerChild != nullptr) {
            workerChild->deleteLater();
        }
    }

    void threadFinished() override
    {
        finishedTid = QThread::currentThreadId();
        callbackOrder << "finished";
        childAliveAtFinished = (workerChild.isNull() == false);
    }
};


/**
 * @brief Probe that terminates itself via finishAndStop() from threadStarted().
 *
 * Exercises the second wind-down path. threadAboutToFinish() is a direct
 * call here (already on worker thread), not a marshalled invokeMethod.
 */
class SelfTerminatingProbe : public AbstractThreadClass
{
    Q_OBJECT
public:
    SelfTerminatingProbe() : AbstractThreadClass("self-terminating-probe") {}

    bool startedCalled = false;
    bool aboutToFinishCalled = false;
    bool finishedCalled = false;
    Qt::HANDLE startedTid = nullptr;
    Qt::HANDLE aboutToFinishTid = nullptr;

protected:
    void threadStarted() override
    {
        startedCalled = true;
        startedTid = QThread::currentThreadId();
        finishAndStop(true, "self-terminate");
    }

    void threadAboutToFinish() override
    {
        aboutToFinishCalled = true;
        aboutToFinishTid = QThread::currentThreadId();
    }

    void threadFinished() override
    {
        finishedCalled = true;
    }
};


/**
 * @brief Subclass that does not override threadAboutToFinish().
 *
 * Verifies the default no-op base implementation is safe — exercising the
 * marshalled invokeMethod path with no work to do.
 */
class MinimalProbe : public AbstractThreadClass
{
    Q_OBJECT
public:
    MinimalProbe() : AbstractThreadClass("minimal-probe") {}

    bool startedCalled = false;

protected:
    void threadStarted() override
    {
        startedCalled = true;
    }
};


/**
 * @brief Holds the worker inside threadAboutToFinish() for a known interval.
 *
 * Lets a caller distinguish a stop() that waited for the wind-down from one that returned while
 * the worker was still inside it.
 */
class SlowWindDownProbe : public AbstractThreadClass
{
    Q_OBJECT
public:
    static const int WindDownMs = 400;

    SlowWindDownProbe() : AbstractThreadClass("slow-winddown") {}

    bool insideWindDown = false;

protected:
    void threadStarted() override {}

    void threadAboutToFinish() override
    {
        insideWindDown = true;
        QThread::msleep(WindDownMs);
        insideWindDown = false;
    }
};


class TstThreadLifecycle : public QObject
{
    Q_OBJECT

private slots:
    void startPath_theOsThreadCarriesTheLogTag()
    {
        // ⚠ Read from /proc rather than from QThread::objectName(). The whole point of this is what
        // the OPERATING SYSTEM reports - a profiler, htop and /proc/<pid>/task are the readers, and
        // asserting on the objectName we just set would pass whether or not Qt used it.
        LifecycleProbe probe;
        QVERIFY(probe.start(TimeSpan::fromSeconds(2)));

        QStringList names;
        QDir tasks("/proc/self/task");
        const QStringList tids = tasks.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for(const QString& tid : tids) {
            QFile file(QString("/proc/self/task/%1/comm").arg(tid));
            if(file.open(QIODevice::ReadOnly)) {
                names.append(QString::fromUtf8(file.readAll()).trimmed());
            }
        }
        QVERIFY(probe.stop(TimeSpan::fromSeconds(2)));

        // The control: without the fix every worker reports the class name and this list is all
        // "QThread", so a test that only checked for the tag could not tell the two apart.
        QVERIFY2(names.contains("lifecycle-probe"),
                 qPrintable(QString("thread names were: %1").arg(names.join(", "))));
        QVERIFY2(names.contains("QThread") == false,
                 qPrintable(QString("a worker is still unnamed: %1").arg(names.join(", "))));
    }

    void threadName_keepsTheEndOfALongTag()
    {
        // ⚠ The tail, not the head. These tags end in the host or node that tells two workers
        // apart, so a head truncation would render every inverter pool identical.
        class LongTagProbe : public AbstractThreadClass
        {
        public:
            LongTagProbe() : AbstractThreadClass("query-pool-inverter-ctrl-127.0.2.2") {}
        protected:
            virtual void threadStarted() override {}
        };

        LongTagProbe probe;
        QCOMPARE(probe.threadName().length(), 15);
        QCOMPARE(probe.threadName(), QString("-ctrl-127.0.2.2"));
    }

    void stopPath_callbackOrder()
    {
        LifecycleProbe probe;
        QVERIFY(probe.start(TimeSpan::fromSeconds(2)));
        QThread::msleep(50);
        QVERIFY(probe.stop(TimeSpan::fromSeconds(2)));

        QStringList expected{"started", "aboutToFinish", "finished"};
        QCOMPARE(probe.callbackOrder, expected);
    }

    void stopPath_runsOnWorkerThread()
    {
        LifecycleProbe probe;
        Qt::HANDLE callerTid = QThread::currentThreadId();

        QVERIFY(probe.start(TimeSpan::fromSeconds(2)));
        QThread::msleep(50);
        QVERIFY(probe.stop(TimeSpan::fromSeconds(2)));

        QVERIFY(probe.startedTid != callerTid);
        QCOMPARE(probe.aboutToFinishTid, probe.startedTid);
        QCOMPARE(probe.finishedTid, probe.startedTid);
    }

    void stopPath_deleteLaterRunsBeforeThreadExit()
    {
        LifecycleProbe probe;
        QVERIFY(probe.start(TimeSpan::fromSeconds(2)));
        QThread::msleep(50);
        QVERIFY(probe.stop(TimeSpan::fromSeconds(2)));

        // The QObject allocated in threadStarted() lives on the worker thread.
        // deleteLater() posted from threadAboutToFinish() should have been
        // processed by the time threadFinished() runs.
        QVERIFY2(probe.childAliveAtFinished == false,
                 "deleteLater() posted from threadAboutToFinish() was not processed "
                 "before threadFinished() — event loop may have exited too early");
    }

    void finishAndStopPath_firesAboutToFinish()
    {
        SelfTerminatingProbe probe;
        QVERIFY(probe.start(TimeSpan::fromSeconds(2)));
        QVERIFY(probe.waitForCompletion(TimeSpan::fromSeconds(2)));

        QVERIFY(probe.startedCalled);
        QVERIFY(probe.aboutToFinishCalled);
        QVERIFY(probe.finishedCalled);

        // finishAndStop() runs on the worker thread; threadAboutToFinish() is
        // a direct call from there, not a cross-thread marshal.
        QCOMPARE(probe.aboutToFinishTid, probe.startedTid);
    }

    void defaultImplementation_isSafe()
    {
        MinimalProbe probe;
        QVERIFY(probe.start(TimeSpan::fromSeconds(2)));
        QThread::msleep(50);
        QVERIFY(probe.stop(TimeSpan::fromSeconds(2)));

        QVERIFY(probe.startedCalled);
    }
    /**
     * ⚠ Every destructor in this codebase calls the bare stop(), and its correctness rests entirely
     * on the default TimeSpan::zero() meaning "wait indefinitely" - MutexEvent::wait() takes the
     * no-timeout branch for any msecs <= 0. Subclass destructors delete the members their worker
     * thread is still reading, so a stop() that returned early would be a use-after-free that only
     * shows up under load, somewhere else, later.
     *
     * ⚠⚠ The Doxygen on stop() says "zero = don't wait". THE DOCUMENTATION IS WRONG, and this test
     * exists because the plausible edit is someone believing it and making the code agree.
     */
    void stopPath_theDefaultTimeoutWaitsForTheWindDownRatherThanNotWaitingAtAll()
    {
        SlowWindDownProbe probe;
        QVERIFY(probe.start(TimeSpan::fromSeconds(2)));

        QElapsedTimer elapsed;
        elapsed.start();
        const bool stopped = probe.stop();          // no argument - the destructors' call
        const qint64 waited = elapsed.elapsed();

        QVERIFY2(stopped, "the bare stop() reported failure");
        QVERIFY2(waited >= SlowWindDownProbe::WindDownMs,
                 qPrintable(QString("stop() returned after %1 ms while the wind-down takes %2 ms - "
                                    "zero timeout is no longer waiting, and every subclass "
                                    "destructor now races its own members")
                            .arg(waited).arg(SlowWindDownProbe::WindDownMs)));
        QVERIFY2(probe.insideWindDown == false, "stop() returned while the worker was still winding down");
        QVERIFY2(probe.isRunning() == false, "stop() returned with the thread still running");
    }

    /**
     * ⚠ The control. Without it, an implementation where stop() simply blocked for a fixed age -
     * or where the probe never ran its wind-down at all - would satisfy the check above.
     */
    void stopPath_aProbeWithNoWindDownReturnsPromptly()
    {
        MinimalProbe probe;
        QVERIFY(probe.start(TimeSpan::fromSeconds(2)));

        QElapsedTimer elapsed;
        elapsed.start();
        QVERIFY(probe.stop());
        const qint64 waited = elapsed.elapsed();

        QVERIFY2(waited < SlowWindDownProbe::WindDownMs,
                 qPrintable(QString("a probe with nothing to wind down took %1 ms to stop, so the "
                                    "test above cannot tell waiting from stalling").arg(waited)));
    }

};

QTEST_MAIN(TstThreadLifecycle)
#include "tst_threadlifecycle.moc"
