#include "daterange.h"

#include <Kanoop/datetimeutil.h>

DateRange::DateRange(const DateRange& other)
{
    *this = other;
}

DateRange DateRange::fromMidpoint(const QDateTime& midpoint, const TimeSpan& duration)
{
    DateRange result(midpoint.addMSecs(-(duration / 2).totalMilliseconds()), midpoint.addMSecs((duration / 2).totalMilliseconds()));
    return result;
}

DateRange& DateRange::operator=(const DateRange& other)
{
    _startTime = other._startTime;
    _endTime = other._endTime;
    return *this;
}

DateRange DateRange::operator+(const TimeSpan& other) const
{
    DateRange range(_startTime.addMSecs(other.totalMilliseconds()), _endTime.addMSecs(other.totalMilliseconds()));
    return range;
}

DateRange DateRange::operator-(const TimeSpan& other) const
{
    DateRange range(_startTime.addMSecs(-other.totalMilliseconds()), _endTime.addMSecs(-other.totalMilliseconds()));
    return range;
}

void DateRange::operator+=(const TimeSpan& other)
{
    _startTime = _startTime.addMSecs(other.totalMilliseconds());
    _endTime = _endTime.addMSecs(other.totalMilliseconds());
}

void DateRange::operator-=(const TimeSpan& other)
{
    _startTime = _startTime.addMSecs(other.totalMilliseconds());
    _endTime = _endTime.addMSecs(other.totalMilliseconds());
}

bool DateRange::operator==(const DateRange& other) const
{
    return _startTime == other._startTime && _endTime == other._endTime;
}

bool DateRange::operator!=(const DateRange& other) const
{
    return !(*this == other);
}

DateRange DateRange::subRange(const DateRange& other) const
{
    DateRange result = *this;
    if(result._startTime < other._startTime) {
        result._startTime = other._startTime;
    }
    if(result._endTime > other._endTime) {
        result._endTime = other._endTime;
    }
    return result._endTime > result._startTime ? result : DateRange();
}

DateRange DateRange::merged(const DateRange& other) const
{
    QDateTime min = _startTime.isValid()
                    ? std::min(_startTime, other._startTime)
                    : other._startTime;
    QDateTime max = _endTime.isValid()
                    ? std::max(_endTime, other._endTime)
                    : other._endTime;
    return DateRange(min, max);
}

QString DateRange::toString() const
{
    return QString("%1 to %2 (%3)")
            .arg(DateTimeUtil::toStandardString(_startTime))
            .arg(DateTimeUtil::toStandardString(_endTime))
            .arg(duration().toString());
}
