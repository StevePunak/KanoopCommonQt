#include "Kanoop/ratemonitor.h"

void RateMonitor::addEvent(int count)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();

    _lock.lock();
    _events.append(RateEvent(now, count));
    _lock.unlock();
}

double RateMonitor::eventsPerSecond()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 startTime = now - _evaluationMsecs;

    double result = 0;
    _lock.lock();

    // get rid of old events
    while(_events.count() > 0 && _events[0]._timestamp < startTime)
        _events.removeFirst();

    // count new events
    qint64 firstEventTime = 0;
    for(const RateEvent& e: _events)
    {
        result += e._count;
        if(firstEventTime == 0)
            firstEventTime = e._timestamp;
    }

    _lock.unlock();

    if(firstEventTime == 0)
        firstEventTime = startTime;

    double secondsEvaluated = (double)(now - firstEventTime) / 1000.0;
    result /= secondsEvaluated;

    return result;
}
