#ifndef MUTEXEVENT_H
#define MUTEXEVENT_H

#include <QWaitCondition>
#include <QMutex>
#include "kanoopcommon.h"

class KANOOP_EXPORT MutexEvent
{
public:
    MutexEvent(bool wakeAllWaiters = false) :
        _isset(false),
        _debug(false),
        _wakeAllWaiters(wakeAllWaiters) {}

    void set();
    void clear();
    bool wait(int msecs = 0);

    void setDebug() { _debug = true; }
    void setWakeAllWaiters(bool value) { _wakeAllWaiters = value; }

private:
    QWaitCondition  _condition;
    QMutex _checkMutex;
    bool _isset;
    bool _debug;
    bool _wakeAllWaiters;
};

#endif // MUTEXEVENT_H
