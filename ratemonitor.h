#ifndef RATEMONITOR_H
#define RATEMONITOR_H

#include <QDateTime>
#include <QList>
#include <QMutex>
#include "kanoopcommon.h"

class KANOOP_EXPORT RateMonitor
{
public:
    RateMonitor();

    void addEvent(int count = 1);
    int eventsPerSecond();

private:
    QList<qint64> _events;
    QMutex _lock;
};

#endif // RATEMONITOR_H
