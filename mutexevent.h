#ifndef MUTEXEVENT_H
#define MUTEXEVENT_H

#include <QWaitCondition>
#include <QMutex>
#include "kanoopcommon.h"

class KANOOP_EXPORT MutexEvent
{
public:
    MutexEvent();

    void set();
    void clear();
    bool wait(int msecs = 0);

    void setDebug() { _debug = true; }

private:
    QWaitCondition  _condition;
    QMutex _checkMutex;
    bool _isset;
    bool _debug;
};

#endif // MUTEXEVENT_H
