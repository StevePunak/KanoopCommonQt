/**
 *  RateMonitor
 *
 *  Calculate the rate at which something is happening (e.g. packets-per-second)
 *
 *  Every time the thing happens, call addEvent().
 *
 *  Use eventsPerSecond() to obtain the actual rate.
 *
 *  setEvaluationTime() sets the sliding time window over which the calculation
 *  is performed.
 *
 *  Stephen Punak, July 08 2019
 */
#ifndef RATEMONITOR_H
#define RATEMONITOR_H

#include <QDateTime>
#include <QList>
#include <QMutex>
#include "kanoopcommon.h"

/**
 * @brief Tracks event counts over a sliding time window to compute a real-time rate.
 *
 * Call addEvent() each time the measured event occurs. Use eventsPerSecond() to
 * retrieve the computed rate over the configured evaluation window.
 */
class KANOOP_EXPORT RateMonitor
{
public:
    /** @brief Construct a RateMonitor with a default 1-second evaluation window. */
    RateMonitor() :
        _evaluationMsecs(1000) {}

    /**
     * @brief Construct a RateMonitor with a custom evaluation window.
     * @param evaluationMsecs Sliding window duration in milliseconds
     */
    RateMonitor(int evaluationMsecs) :
        _evaluationMsecs(evaluationMsecs) {}

public slots:
    /**
     * @brief Record one or more occurrences of the monitored event.
     * @param count Number of events to record (default 1)
     */
    void addEvent(int count = 1);

public:
    /**
     * @brief Compute the current event rate over the evaluation window.
     * @return Number of events per second
     */
    double eventsPerSecond();

    /**
     * @brief Return the arbitrary data payload attached to this monitor.
     * @return Stored QVariant data
     */
    QVariant data() const { return _data; }

    /**
     * @brief Attach arbitrary data to this monitor for caller use.
     * @param value Data to store
     */
    void setData(QVariant value) { _data = value; }

    /**
     * @brief Return the evaluation window duration in milliseconds.
     * @return Evaluation window in milliseconds
     */
    int evaluationMsecs() const { return _evaluationMsecs; }

    /**
     * @brief Set the evaluation window duration.
     * @param msecs New window duration in milliseconds
     */
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

    void pruneOldEvents(qint64 now);

    int _evaluationMsecs = 1000;
    QVariant _data;

    QList<RateEvent> _events;
    QMutex _lock;
    qint64 _lastPruneTime = 0;
    static const qint64 PruneIntervalMsecs = 1000;
};

#endif // RATEMONITOR_H
