#ifndef RATEMONITOR_H
#define RATEMONITOR_H

#include <QDateTime>
#include <QList>
#include <QMutex>
#include "kanoopcommon.h"

class KANOOP_EXPORT RateMonitor
{
public:
    RateMonitor() :
        _evaluationMsecs(1000) {}

    RateMonitor(int evaluationMsecs) :
        _evaluationMsecs(evaluationMsecs) {}

    void addEvent(int count = 1);
    double eventsPerSecond();

    void setEvaluationTime(int msecs) { _evaluationMsecs = msecs; }

private:
    class RateEvent
    {
        friend class RateMonitor;
    public:
        RateEvent() :
            _timestamp(0),
            _count(0) {}
        RateEvent(quint64 timestamp, qint64 count) :
            _timestamp(timestamp),
            _count(count) {}
    private:
        qint64 _timestamp;
        qint64 _count;
    };
    int _evaluationMsecs;

    QList<RateEvent> _events;
    QMutex _lock;
};

#endif // RATEMONITOR_H
