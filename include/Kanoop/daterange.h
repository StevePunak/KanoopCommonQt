#ifndef DATERANGE_H
#define DATERANGE_H
#include <QDateTime>

#include <Kanoop/timespan.h>
#include "kanoopcommon.h"

class KANOOP_EXPORT DateRange
{
public:
    DateRange() {}
    DateRange(const QDateTime& startTime, const QDateTime& endTime) :
        _startTime(startTime), _endTime(endTime) {}
    DateRange(const QDateTime& startTime, const TimeSpan& duration) :
        _startTime(startTime), _endTime(startTime.addMSecs(duration.totalMilliseconds())) {}

    DateRange(const DateRange& other);

    static DateRange fromMidpoint(const QDateTime& midpoint, const TimeSpan& duration);

    DateRange& operator=(const DateRange& other);

    DateRange operator+(const TimeSpan& other) const;
    DateRange operator-(const TimeSpan& other) const;
    void operator+=(const TimeSpan& other);
    void operator-=(const TimeSpan& other);

    bool operator==(const DateRange& other) const;
    bool operator!=(const DateRange& other) const;

    QDateTime startTime() const { return _startTime; }
    void setStartTime(const QDateTime& value) { _startTime = value; }

    QDateTime endTime() const { return _endTime; }
    void setEndTime(const QDateTime& value) { _endTime = value; }

    TimeSpan duration() const { return TimeSpan::absDiff(_startTime, _endTime); }
    QDateTime midpoint() const { return _startTime.addMSecs((duration() / 2).totalMilliseconds()); }

    bool contains(const QDateTime& time) const { return time >= _startTime && time <= _endTime; }
    bool contains(const DateRange& other) const { return contains(other.startTime()) && contains(other.endTime()); }

    DateRange subRange(const DateRange& other) const;
    DateRange merged(const DateRange& other) const;

    static DateRange merged(const DateRange& a, const DateRange& b) { return a.merged(b); }

    QString toString() const;

    bool isValid() const { return _startTime.isValid() && _endTime.isValid(); }

    class List : public QList<DateRange>
    {
    public:
        List consolidated() const
        {
            List result;
            List sorted = *this;
            std::sort(sorted.begin(), sorted.end(), [](const DateRange& a, const DateRange& b) { return a.startTime() < b.startTime(); });

            if(sorted.count() < 2) {
                return sorted;
            }

            DateRange current = first();

            for (int i = 1; i < size(); ++i) {
                const DateRange& next = (*this)[i];

                if (current.endTime() >= next.startTime()) {
                    current = DateRange(
                                  current.startTime(),
                                  std::max(current.endTime(), next.endTime())
                                  );
                }
                else {
                    result.append(current);
                    current = next;
                }
            }
            result.append(current);

            return result;
        }
    };

private:
    QDateTime _startTime;
    QDateTime _endTime;
};

#endif // DATERANGE_H
