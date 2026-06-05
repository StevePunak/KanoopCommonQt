/**
 * @brief Abstract base class for QObject-based workers that run in a dedicated QThread.
 */
#ifndef ABSTRACTTHREADCLASS_H
#define ABSTRACTTHREADCLASS_H

#include <QAtomicInt>
#include <QMutex>
#include <QObject>
#include <QTextStream>
#include <QThread>

#include <Kanoop/mutexevent.h>
#include <Kanoop/log.h>
#include <Kanoop/kanoopcommon.h>

#include "loggingbaseclass.h"

/**
 * @brief Manages the lifecycle of a QObject running in its own QThread.
 *
 * Subclass AbstractThreadClass and implement threadStarted() to perform work on the
 * worker thread. The thread is created and destroyed automatically by start() and stop().
 * The started() and finished() signals notify the main thread of state changes.
 */
class KANOOP_EXPORT AbstractThreadClass : public QObject,
                                          public LoggingBaseClass
{
    Q_OBJECT

public:
    /**
     * @brief Construct with an optional log category and parent.
     * @param category Log category for this class (default: uncategorized)
     * @param parent Optional QObject parent
     */
    AbstractThreadClass(const Log::LogCategory& category = Log::LogCategory());

    /**
     * @brief Construct with a category name string and optional parent.
     * @param category Category name string
     * @param parent Optional QObject parent
     */
    AbstractThreadClass(const QString& category, QObject* parent = nullptr);

    /** @brief Destructor — stops the thread if still running. */
    virtual ~AbstractThreadClass();

    /**
     * @brief Start the worker thread and optionally wait for it to signal readiness.
     * @param timeout How long to wait for the thread to start (zero = don't wait)
     * @return true if the thread started successfully
     */
    virtual bool start(const TimeSpan& timeout = TimeSpan::zero());

    /**
     * @brief Request the worker thread to stop and optionally wait for it to finish.
     * @param timeout How long to wait for the thread to finish (zero = don't wait)
     * @return true if the thread stopped within the timeout
     */
    virtual bool stop(const TimeSpan &timeout = TimeSpan::zero());

    /** @brief Immediately interrupt the worker thread (sets the stopping flag). */
    virtual void abort();

    /**
     * @brief Block until the worker thread finishes or the timeout elapses.
     * @param timeout Maximum wait duration (zero = wait indefinitely)
     * @return true if the thread finished within the timeout
     */
    virtual bool waitForCompletion(const TimeSpan& timeout = TimeSpan::zero());

    /**
     * @brief Test whether the worker thread is currently running.
     * @return true if the thread is running
     */
    bool isRunning() const { return _thread.isRunning(); }

    /**
     * @brief Test whether stop() has been called and the thread is winding down.
     * @return true if stopping is in progress
     */
    bool isStopping() const { return _stopping.loadRelaxed() != 0; }

    /**
     * @brief Return whether the last run completed successfully.
     * @return true if finishAndStop() was called with success = true
     */
    bool success() const { return _success; }

    /**
     * @brief Return the completion message set by finishAndStop().
     * @return Completion message string
     */
    QString completionMessage() const { return _message; }

    /**
     * @brief Control whether start() blocks until the thread signals readiness.
     * @param value true to make start() blocking
     */
    void setBlockingStart(bool value) { _blockingStart = value; }

    /**
     * @brief Return the number of AbstractThreadClass instances currently alive.
     *
     * Diagnostic counter. Note that reading this together with runningThreadCount()
     * is not atomic as a pair — the two values may be momentarily inconsistent.
     * @return Count of live instances across all threads
     */
    static int instanceCount() { return _InstanceCount.loadRelaxed(); }

    /**
     * @brief Return the number of AbstractThreadClass worker threads currently running.
     * @return Count of threads between onThreadStarted() and onThreadFinished()
     */
    static int runningThreadCount() { return _RunningThreadCount.loadRelaxed(); }

protected:
    /**
     * @brief Entry point called on the worker thread immediately after it starts.
     *
     * Subclasses must implement this method to perform their work. When done,
     * call finishAndStop() to signal completion.
     */
    virtual void threadStarted() = 0;

    /**
     * @brief Called on the worker thread, with the event loop still alive,
     *        just before the thread is asked to quit.
     *
     * Override to perform thread-affine cleanup that must complete while the
     * event loop is still pumping — for example, disconnecting QObject-based
     * devices (QCanBusDevice, QModbusClient) whose thread affinity is the
     * worker thread.
     *
     * This is also the right place to call deleteLater() on any QObjects
     * owned by the worker thread. Three properties line up here that don't
     * line up at any other lifecycle point: the call runs on the owning
     * thread, the event loop is still pumping so DeferredDelete events get
     * processed, and the marshalled call is synchronous from the caller's
     * perspective so the deletions are queued before quit() is issued.
     *
     * The default implementation does nothing.
     */
    virtual void threadAboutToFinish() {}

    /** @brief Called on the worker thread just before it exits. */
    virtual void threadFinished() {}

    /**
     * @brief Block until the thread has entered threadStarted() or the timeout elapses.
     * @param timeout Maximum wait duration
     * @return true if the thread started within the timeout
     */
    virtual bool waitForStart(const TimeSpan& timeout);

    /**
     * @brief Signal that the worker has finished, recording success and a message.
     * @param success Whether the work completed successfully
     * @param message Optional human-readable completion message
     */
    virtual void finishAndStop(bool success, const QString& message = QString());

    /**
     * @brief Write a line to the standard output stream.
     * @param line Text to write
     */
    void writeLine(const QString& line) { _stdout << line << Qt::endl; }

    /**
     * @brief Write a line to the standard error stream.
     * @param line Text to write
     */
    void writeErrorLine(const QString& line) { _stderr << line << Qt::endl; }

private:
    /** @brief Shared initialisation for all constructors. */
    void commonInit();

    /** @brief Helper that calls threadAboutToFinish() with exception handling. */
    void invokeThreadAboutToFinish();

    /** @brief Worker-side helper queued by stop(): runs the about-to-finish hook, then
     *  quits the worker's event loop. Sequencing both on the worker guarantees the hook
     *  executes before the loop exits without the caller having to block. */
    void invokeThreadAboutToFinishAndQuit();

    bool _success = false;
    QString _message;

    /** Written by stop() on the caller's thread and by finishAndStop() on the worker —
     *  atomic so the cross-thread first-completion-wins handoff is a real test-and-set
     *  rather than a data race. 0 = not stopping, 1 = stopping. */
    QAtomicInt _stopping;
    bool _blockingStart = false;

    QThread _thread;
    QTextStream _stdout;
    QTextStream _stderr;

    MutexEvent _startEvent;
    MutexEvent _stopEvent;

    // Diagnostic counters (relaxed ordering — sufficient for monotonic counting)
    static QAtomicInt _InstanceCount;
    static QAtomicInt _RunningThreadCount;

signals:
    /** @brief Emitted when the worker thread has started. */
    void started();
    /** @brief Emitted when the worker thread has finished. */
    void finished();

private slots:
    /** @brief Invoked by QThread when the thread enters its event loop. */
    void onThreadStarted();
    /** @brief Invoked by QThread when the thread exits its event loop. */
    void onThreadFinished();
};

#endif // ABSTRACTTHREADCLASS_H
