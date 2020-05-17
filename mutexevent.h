#ifndef MUTEXEVENT_H
#define MUTEXEVENT_H

#include <QWaitCondition>
#include <QMutex>
#include "kanoopcommon.h"

class KANOOP_EXPORT MutexEvent
{
public:
    MutexEvent() {}

    void set();
    bool wait(int msecs = 0);

private:
    QWaitCondition  _condition;
};

#endif // MUTEXEVENT_H
