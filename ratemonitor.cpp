#include "ratemonitor.h"

#define EVALUATION_PERIOD_MS    1000

RateMonitor::RateMonitor()
{
}

void RateMonitor::addEvent(int count)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    _lock.lock();

    while(count--)
        _events.append(now);

    _lock.unlock();
}

int RateMonitor::eventsPerSecond()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 base = now - EVALUATION_PERIOD_MS;

    int result = 0;
    _lock.lock();

    while(_events.count() > 0 && _events[0] < base)
        _events.removeFirst();

    result = _events.count();

    _lock.unlock();

    return result;
}
