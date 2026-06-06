#include "utility/abstractthreadclass.h"
#include "commonexception.h"

#include <QCoreApplication>

QAtomicInt AbstractThreadClass::_InstanceCount = 0;
QAtomicInt AbstractThreadClass::_RunningThreadCount = 0;

AbstractThreadClass::AbstractThreadClass(const Log::LogCategory &category) :
    QObject(),
    LoggingBaseClass(category),
    _success(false),
    _stdout(stdout), _stderr(stderr)
{
    commonInit();
}

AbstractThreadClass::AbstractThreadClass(const QString &category, QObject *parent) :
    QObject(parent),
    LoggingBaseClass(category),
    _success(false),
    _stdout(stdout), _stderr(stderr)
{
    commonInit();
}

void AbstractThreadClass::commonInit()
{
    _InstanceCount.fetchAndAddRelaxed(1);

    AbstractThreadClass::setObjectName(AbstractThreadClass::metaObject()->className());

    connect(&_thread, &QThread::started, this, &AbstractThreadClass::onThreadStarted);
    connect(&_thread, &QThread::finished, this, &AbstractThreadClass::onThreadFinished);
    moveToThread(&_thread);
    _startEvent.setWakeAllWaiters(true);
}

AbstractThreadClass::~AbstractThreadClass()
{
    logText(LVL_DEBUG, QString("TEARDOWN %1: dtor enter — running=%2 onOwnThread=%3 caller=0x%4")
            .arg(objectName())
            .arg(_thread.isRunning())
            .arg(QThread::currentThread() == &_thread)
            .arg((quintptr)QThread::currentThread(), 0, 16));
    if(_thread.isRunning() && QThread::currentThread() == &_thread) {
        logText(LVL_ERROR, QString("TEARDOWN %1: dtor on own running thread — ~QThread will self-join and hang").arg(objectName()));
    }
    if(_thread.isRunning()) {
        try
        {
            _thread.quit();
            if(!_thread.wait(500)) {
                logText(LVL_DEBUG, "Thread never joined");
            }
            if(!_stopEvent.wait(500)) {
                logText(LVL_DEBUG, "Stop Event never fired");
            }
        }
        catch(...)
        {
            logText(LVL_DEBUG, "Thread termination exception");
        }
    }
    else {
        // isRunning() reports false for the entire finish phase
        // (QThreadPrivate::isInFinish) while onThreadFinished() is still
        // executing member functions on the worker — deleting on that
        // report alone is a use-after-free. Join unconditionally: wait()
        // blocks until the finish phase fully completes, and returns
        // immediately for a never-started or long-finished thread.
        _thread.wait();
        _stopEvent.set();
    }
    _InstanceCount.fetchAndSubRelaxed(1);
    logText(LVL_DEBUG, QString("TEARDOWN %1: dtor exit").arg(objectName()));
}

bool AbstractThreadClass::start(const TimeSpan &timeout)
{
    bool result = true;
    if(_thread.isRunning()) {
        logText(LVL_WARNING, QString("%1: Tried to start while already running").arg(objectName()));
    }
    else {
        _thread.start();
        if((_blockingStart || timeout != TimeSpan::zero()) && _startEvent.wait(timeout) == false) {
            logText(LVL_ERROR, QString("%1: Thread start never completed").arg(objectName()));
            result = false;
        }
    }
    return result;
}

bool AbstractThreadClass::stop(const TimeSpan& timeout)
{
    bool result = true;
    if(_thread.isRunning() == false) {
        logText(LVL_WARNING, QString("%1: Tried to stop while not running").arg(objectName()));
        // Not-running includes the finish phase, where the worker is still
        // executing onThreadFinished(). Join so the caller can safely delete
        // this object on return.
        _thread.wait();
    }
    else {
        if(QThread::currentThread() == &_thread) {
            _stopping.storeRelaxed(1);
            invokeThreadAboutToFinish();
            _thread.quit();
        }
        else if(_stopping.testAndSetOrdered(0, 1)) {
            // We won the stop race (the atomic test-and-set closes the window where this
            // and a concurrent finishAndStop() both pass a plain check before either sets
            // the flag). Queue the about-to-finish hook + quit as a single worker-side
            // call. A blocking invoke here deadlocks the caller forever when the worker
            // has already quit its event loop via finishAndStop() — the invoke is never
            // serviced. Queuing hook-then-quit together also guarantees a live loop
            // runs the hook before exiting (a quit() posted from this thread would
            // interrupt the loop without draining the queued hook). If the loop is
            // already gone, the event is simply dropped — finishAndStop() has run
            // the hook and quit already. _stopEvent.wait() below synchronizes.
            QMetaObject::invokeMethod(this, &AbstractThreadClass::invokeThreadAboutToFinishAndQuit);
        }
        else {
            // Worker is already shutting itself down (finishAndStop): the hook has
            // run on the worker and quit() has been called. Just wait for the exit.
            _thread.quit();
        }
        if(_stopEvent.wait(timeout) == false) {
            logText(LVL_ERROR, QString("%1: Thread stop never completed. Aborting thread!").arg(objectName()));
            _thread.terminate();
            result = false;
        }
    }
    _stopping.storeRelaxed(0);
    return result;
}

void AbstractThreadClass::abort()
{
    logText(LVL_WARNING, QString("%1: Aborting thread").arg(objectName()));
    _thread.terminate();
    _thread.wait();
    logText(LVL_WARNING, QString("%1: Thread terminated").arg(objectName()));
}

bool AbstractThreadClass::waitForCompletion(const TimeSpan &timeout)
{
    bool result;
    if((result = _stopEvent.wait(timeout)) == false) {
        logText(LVL_WARNING, QString("%1: Thread never finished").arg(objectName()));
    }
    return result;
}

bool AbstractThreadClass::waitForStart(const TimeSpan& timeout)
{
    return _startEvent.wait(timeout);
}

void AbstractThreadClass::finishAndStop(bool success, const QString &message)
{
    logText(LVL_DEBUG, QString("TEARDOWN %1: finishAndStop — quitting own thread (success=%2)")
            .arg(objectName()).arg(success));
    // First completion wins. A second call lands when a signal carrying a
    // result is already queued behind a self-initiated quit (e.g. an HTTP
    // reply's finished signal after a status-driven finishAndStop, or a late
    // success racing a timeout failure) — don't stomp the recorded result or
    // re-run the about-to-finish hook. The atomic test-and-set also closes the
    // race against a concurrent cross-thread stop().
    if(_stopping.testAndSetOrdered(0, 1) == false) {
        return;
    }
    _success = success;
    _message = message;
    invokeThreadAboutToFinish();
    _thread.quit();
}

void AbstractThreadClass::onThreadStarted()
{
    _RunningThreadCount.fetchAndAddRelaxed(1);
    try
    {
        emit started();
        threadStarted();
    }
    catch(const CommonException& e)
    {
        logText(LVL_ERROR, QString("%1: Thread start exception: %2").arg(objectName()).arg(e.message()));
    }
    _startEvent.set();
}

void AbstractThreadClass::onThreadFinished()
{
    try
    {
        threadFinished();
    }
    catch(const CommonException& e)
    {
        logText(LVL_ERROR, QString("%1: Thread start exception: %2").arg(objectName()).arg(e.message()));
    }
    _stopEvent.set();
    emit finished();
    _stopping.storeRelaxed(0);
    _RunningThreadCount.fetchAndSubRelaxed(1);
}

void AbstractThreadClass::invokeThreadAboutToFinish()
{
    try
    {
        threadAboutToFinish();
    }
    catch(const CommonException& e)
    {
        logText(LVL_ERROR, QString("%1: threadAboutToFinish exception: %2").arg(objectName()).arg(e.message()));
    }
    // Drain any deleteLater() events posted from the hook before returning.
    // QThread emits finished() *before* its own DeferredDelete drain in
    // QThreadPrivate::finish(), and the upcoming quit() event outranks the
    // low-priority DeferredDelete in the worker's queue — so without this
    // call, threadFinished() runs before the deletions are processed.
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
}

void AbstractThreadClass::invokeThreadAboutToFinishAndQuit()
{
    invokeThreadAboutToFinish();
    _thread.quit();
}


#include "Kanoop/utility/moc_abstractthreadclass.cpp"
