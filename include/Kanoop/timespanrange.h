#ifndef TIMESPANRANGE_H
#define TIMESPANRANGE_H
#include <Kanoop/timespan.h>
#include "kanoopcommon.h"

class DateRange;
class KANOOP_EXPORT TimeSpanRange
{
public:
    TimeSpanRange() {}
    TimeSpanRange(const TimeSpan& startOffset, const TimeSpan& endOffset) :
        _startOffset(startOffset), _endOffset(endOffset) {}
    TimeSpanRange(const QDateTime& startTime, const TimeSpan& duration) :
        _startOffset(TimeSpan::fromMilliseconds(startTime.toMSecsSinceEpoch())),
        _endOffset(TimeSpan::fromMilliseconds(startTime.toMSecsSinceEpoch()) + duration) {}

    TimeSpanRange(const TimeSpanRange& other);

    static TimeSpanRange fromMidpoint(const TimeSpan& midpoint, const TimeSpan& duration);
    static TimeSpanRange fromDateRange(const DateRange& dateRange);
    static TimeSpanRange fromStartAndDuration(const TimeSpan& startOffset, const TimeSpan& duration);

    TimeSpanRange& operator=(const TimeSpanRange& other);

    TimeSpanRange operator+(const TimeSpan& other) const;
    TimeSpanRange operator-(const TimeSpan& other) const;
    void operator+=(const TimeSpan& other);
    void operator-=(const TimeSpan& other);

    bool operator==(const TimeSpanRange& other) const;
    bool operator!=(const TimeSpanRange& other) const;

    TimeSpan startOffset() const { return _startOffset; }
    void setStartOffset(const TimeSpan& value) { _startOffset = value; }

    TimeSpan endOffset() const { return _endOffset; }
    void setEndOffset(const TimeSpan& value) { _endOffset = value; }

    TimeSpan duration() const { return _endOffset - _startOffset; }
    TimeSpan midpoint() const { return _startOffset + (duration() / 2); }

    bool contains(const TimeSpan& time) const { return time >= _startOffset && time <= _endOffset; }
    bool contains(const TimeSpanRange& other) const { return contains(other.startOffset()) && contains(other.endOffset()); }

    TimeSpanRange subRange(const TimeSpanRange& other) const;
    TimeSpanRange merged(const TimeSpanRange& other) const;

    static TimeSpanRange merged(const TimeSpanRange& a, const TimeSpanRange& b) { return a.merged(b); }

    QString toString() const;

    bool isValid() const { return _startOffset.isValid() && _endOffset.isValid(); }

    class List : public QList<TimeSpanRange>
    {
    public:
        List consolidated() const
        {
            List result;
            List sorted = *this;
            std::sort(sorted.begin(), sorted.end(), [](const TimeSpanRange& a, const TimeSpanRange& b) { return a.startOffset() < b.startOffset(); });

            if(sorted.count() < 2) {
                return sorted;
            }

            TimeSpanRange current = first();

            for (int i = 1; i < size(); ++i) {
                const TimeSpanRange& next = (*this)[i];

                if (current.endOffset() >= next.startOffset()) {
                    current = TimeSpanRange(
                                  current.startOffset(),
                                  std::max(current.endOffset(), next.endOffset())
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
    TimeSpan _startOffset;
    TimeSpan _endOffset;
};

#endif // TIMESPANRANGE_H
