#include "Kanoop/ratemonitor.h"

void RateMonitor::addEvent(int count)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();

    _lock.lock();
    _events.append(RateEvent(now, count));
    if(now - _lastPruneTime > PruneIntervalMsecs) {
        pruneOldEvents(now);
    }
    _lock.unlock();
}

double RateMonitor::eventsPerSecond()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();

    double result = 0;
    _lock.lock();

    pruneOldEvents(now);

    // count events
    qint64 firstEventTime = 0;
    for(const RateEvent& e: _events)
    {
        result += e._count;
        if(firstEventTime == 0)
            firstEventTime = e._timestamp;
    }

    _lock.unlock();

    if(firstEventTime == 0)
        firstEventTime = now - _evaluationMsecs;

    double secondsEvaluated = (double)(now - firstEventTime) / 1000.0;
    result /= secondsEvaluated;

    return result;
}

void RateMonitor::pruneOldEvents(qint64 now)
{
    qint64 startTime = now - _evaluationMsecs;
    while(_events.count() > 0 && _events[0]._timestamp < startTime) {
        _events.removeFirst();
    }
    _lastPruneTime = now;
}
