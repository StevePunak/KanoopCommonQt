/**
 *  MutexEvent
 *
 *  A very useful object for multi-threaded applications which wish to be
 *  be event driven.
 *
 *  It is based on the C# AutoResetEvent class which combines a mutex and
 *  a cond_var.
 *
 *  The event driven thread call wait() with an optional timeout.
 *
 *  The event generating thread calls set() and the waiting thread wakes up.
 *
 *  Stephen Punak, May 15 2020
 */
#ifndef MUTEXEVENT_H
#define MUTEXEVENT_H

#include <QWaitCondition>
#include <QMutex>
#include "kanoopcommon.h"
#include "timespan.h"

/**
 * @brief A mutex/condition-variable pair modelling the C# AutoResetEvent.
 *
 * One thread calls wait() and blocks until another thread calls set().
 * The event auto-resets after each wakeup unless wakeAllWaiters is enabled,
 * in which case all blocked waiters are woken simultaneously.
 */
class KANOOP_EXPORT MutexEvent
{
public:
    /**
     * @brief Construct a MutexEvent.
     * @param wakeAllWaiters If true, set() will wake all waiting threads instead of just one
     */
    MutexEvent(bool wakeAllWaiters = false) :
        _isset(false),
        _isWaiting(false),
        _debug(false),
        _wakeAllWaiters(wakeAllWaiters) {}

    /** @brief Virtual destructor. */
    virtual ~MutexEvent() {}

    /** @brief Signal the event, waking one (or all) waiting thread(s). */
    void set();

    /** @brief Reset the event to the unsignalled state. */
    void clear();

    /**
     * @brief Block the calling thread until the event is set or the timeout elapses.
     * @param msecs Maximum wait time in milliseconds; 0 means wait indefinitely
     * @return true if the event was set, false if the timeout elapsed
     */
    bool wait(int msecs = 0);

    /**
     * @brief Block the calling thread until the event is set or the timeout elapses.
     * @param timeout Maximum wait duration
     * @return true if the event was set, false if the timeout elapsed
     */
    bool wait(const TimeSpan& timeout)
    {
        return wait(timeout.totalMilliseconds());
    }

    /**
     * @brief Test whether a thread is currently blocked in wait().
     * @return true if a thread is waiting
     */
    bool isWaiting() const { return _isWaiting; }

    /**
     * @brief Return the arbitrary data payload attached to this event.
     * @return Stored QVariant data
     */
    QVariant data() const { return _data; }

    /**
     * @brief Attach arbitrary data to this event for use by the woken thread.
     * @param value Data to store
     */
    void setData(const QVariant& value) { _data = value; }

    /** @brief Enable verbose debug output for this event. */
    void setDebug() { _debug = true; }

    /**
     * @brief Control whether set() wakes all waiting threads or just one.
     * @param value If true, all waiters are woken; if false, only one is woken
     */
    void setWakeAllWaiters(bool value) { _wakeAllWaiters = value; }

private:
    QWaitCondition  _condition;
    QMutex _checkMutex;
    bool _isset;
    bool _isWaiting;
    bool _debug;
    bool _wakeAllWaiters;

    QVariant _data;
};

#endif // MUTEXEVENT_H
