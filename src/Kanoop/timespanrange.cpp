#include "timespanrange.h"

#include <daterange.h>

TimeSpanRange::TimeSpanRange(const TimeSpanRange& other)
{
    *this = other;
}

TimeSpanRange TimeSpanRange::fromMidpoint(const TimeSpan& midpoint, const TimeSpan& duration)
{
    TimeSpanRange result(midpoint - (duration / 2), midpoint + (duration / 2));
    return result;
}

TimeSpanRange TimeSpanRange::fromDateRange(const DateRange& dateRange)
{
    return TimeSpanRange(dateRange.startTime(), dateRange.duration());
}

TimeSpanRange TimeSpanRange::fromStartAndDuration(const TimeSpan& startOffset, const TimeSpan& duration)
{
    return TimeSpanRange(startOffset, startOffset + duration);
}

TimeSpanRange& TimeSpanRange::operator=(const TimeSpanRange& other)
{
    _startOffset = other._startOffset;
    _endOffset = other._endOffset;
    return *this;
}

TimeSpanRange TimeSpanRange::operator+(const TimeSpan& other) const
{
    TimeSpanRange range(_startOffset + other, _endOffset + other);
    return range;
}

TimeSpanRange TimeSpanRange::operator-(const TimeSpan& other) const
{
    TimeSpanRange range(_startOffset - other, _endOffset - other);
    return range;
}

void TimeSpanRange::operator+=(const TimeSpan& other)
{
    _startOffset += other;
    _endOffset += other;
}

void TimeSpanRange::operator-=(const TimeSpan& other)
{
    _startOffset -= other;
    _endOffset == other;
}

bool TimeSpanRange::operator==(const TimeSpanRange& other) const
{
    return _startOffset == other._startOffset && _endOffset == other._endOffset;
}

bool TimeSpanRange::operator!=(const TimeSpanRange& other) const
{
    return !(*this == other);
}

TimeSpanRange TimeSpanRange::subRange(const TimeSpanRange& other) const
{
    TimeSpanRange result = *this;
    if(result._startOffset < other._startOffset) {
        result._startOffset = other._startOffset;
    }
    if(result._endOffset > other._endOffset) {
        result._endOffset = other._endOffset;
    }
    return result._endOffset > result._startOffset ? result : TimeSpanRange();
}

TimeSpanRange TimeSpanRange::merged(const TimeSpanRange& other) const
{
    TimeSpan min = _startOffset.isValid()
                    ? std::min(_startOffset, other._startOffset)
                    : other._startOffset;
    TimeSpan max = _endOffset.isValid()
                    ? std::max(_endOffset, other._endOffset)
                    : other._endOffset;
    return TimeSpanRange(min, max);
}

QString TimeSpanRange::toString() const
{
    return QString("%1 to %2 (%3)")
            .arg(_startOffset.toString())
            .arg(_endOffset.toString())
            .arg(duration().toString());
}
