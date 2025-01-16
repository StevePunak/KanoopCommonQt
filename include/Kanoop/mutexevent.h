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

class KANOOP_EXPORT MutexEvent
{
public:
    MutexEvent(bool wakeAllWaiters = false) :
        _isset(false),
        _isWaiting(false),
        _debug(false),
        _wakeAllWaiters(wakeAllWaiters) {}
    virtual ~MutexEvent() {}

    void set();
    void clear();
    bool wait(int msecs = 0);
    bool wait(const TimeSpan& timeout)
    {
        return wait(timeout.totalMilliseconds());
    }

    bool isWaiting() const { return _isWaiting; }

    void setDebug() { _debug = true; }
    void setWakeAllWaiters(bool value) { _wakeAllWaiters = value; }

private:
    QWaitCondition  _condition;
    QMutex _checkMutex;
    bool _isset;
    bool _isWaiting;
    bool _debug;
    bool _wakeAllWaiters;
};

#endif // MUTEXEVENT_H
