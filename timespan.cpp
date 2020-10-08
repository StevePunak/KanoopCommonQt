#include "timeconstants.h"
#include "timespan.h"

#include <QTextStream>

const uint64_t TimeSpan::MicrosecondsPerMillisecond    = __MicrosecondsPerMillisecond;
const uint64_t TimeSpan::MicrosecondsPerSecond         = __MicrosecondsPerSecond;
const uint64_t TimeSpan::MicrosecondsPerMinute         = __MicrosecondsPerMinute;
const uint64_t TimeSpan::MicrosecondsPerHour           = __MicrosecondsPerHour;
const uint64_t TimeSpan::MicrosecondsPerDay            = __MicrosecondsPerDay;

const uint64_t TimeSpan::MillisecondsPerSecond         = __MillisecondsPerSecond;
const uint64_t TimeSpan::MillisecondsPerMinute         = __MillisecondsPerMinute;
const uint64_t TimeSpan::MillisecondsPerHour           = __MillisecondsPerHour;
const uint64_t TimeSpan::MillisecondsPerDay            = __MillisecondsPerDay;

const uint64_t TimeSpan::SecondsPerMinute              = __SecondsPerMinute;
const uint64_t TimeSpan::SecondsPerHour                = __SecondsPerHour;
const uint64_t TimeSpan::SecondsPerDay                 = __SecondsPerDay;

const uint64_t TimeSpan::MinutesPerHour                = __MinutesPerHour;
const uint64_t TimeSpan::MinutesPerDay                 = __MinutesPerDay;

const uint64_t TimeSpan::HoursPerDay                   = __HoursPerDay;

const double TimeSpan::DaysPerYear                     = __DaysPerYear;

TimeSpan::TimeSpan(const TimeSpan& other)
{
    *this = other;
}

TimeSpan::TimeSpan(const timespec& other)
{
    *this = other;
}

TimeSpan& TimeSpan::operator =(const timespec& other)
{
    uint64_t msecs = (uint64_t)(other.tv_sec) * 1000L;
    msecs += (uint64_t)other.tv_nsec / 1000000L;
    loadFromMilliseconds(msecs);
    return *this;
}

TimeSpan& TimeSpan::operator =(const TimeSpan& other)
{
    _days = other._days;
    _hours = other._hours;
    _minutes = other._minutes;
    _seconds = other._seconds;
    _milliseconds = other._milliseconds;

    return *this;
}

double TimeSpan::totalSeconds() const
{
    double result = totalMilliseconds() / MillisecondsPerSecond;
    return result;
}

double TimeSpan::totalMinutes() const
{
    double result = totalMilliseconds() / MillisecondsPerMinute;
    return result;
}

double TimeSpan::totalHours() const
{
    double result = totalMilliseconds() / MillisecondsPerHour;
    return result;
}

double TimeSpan::totalDays() const
{
    double result = totalMilliseconds() / MillisecondsPerDay;
    return result;
}

double TimeSpan::totalMilliseconds() const
{
    return
            (_days * MillisecondsPerDay) +
            (_hours * MillisecondsPerHour) +
            (_minutes * MillisecondsPerMinute) +
            (_seconds * MillisecondsPerSecond) + _milliseconds;
}

TimeSpan TimeSpan::operator +(const TimeSpan& other) const
{
    TimeSpan temp;
    temp.loadFromMilliseconds((uint64_t)((uint64_t)totalMilliseconds() + (uint64_t)other.totalMilliseconds()));
    return temp;
}

TimeSpan TimeSpan::operator -(const TimeSpan& other) const
{
    TimeSpan temp;
    temp.loadFromMilliseconds((uint64_t)((uint64_t)totalMilliseconds() - (uint64_t)other.totalMilliseconds()));
    return temp;
}

void TimeSpan::operator +=(const TimeSpan& other)
{
    *this = *this + other;
}

void TimeSpan::operator -=(const TimeSpan& other)
{
    *this = *this - other;
}

bool TimeSpan::operator ==(const TimeSpan& other) const
{
    return totalMilliseconds() == other.totalMilliseconds();
}

bool TimeSpan::operator !=(const TimeSpan& other) const
{
    return totalMilliseconds() != other.totalMilliseconds();
}

bool TimeSpan::operator >(const TimeSpan& other) const
{
    return totalMilliseconds() > other.totalMilliseconds();
}

bool TimeSpan::operator <(const TimeSpan& other) const
{
    return totalMilliseconds() < other.totalMilliseconds();
}

bool TimeSpan::operator >=(const TimeSpan& other) const
{
    return totalMilliseconds() >= other.totalMilliseconds();
}

bool TimeSpan::operator <=(const TimeSpan& other) const
{
    return totalMilliseconds() <= other.totalMilliseconds();
}

TimeSpan TimeSpan::fromMilliseconds(int milliseconds)
{
    TimeSpan ts;
    ts.loadFromMilliseconds(milliseconds);
    return ts;
}

TimeSpan TimeSpan::fromSeconds(int seconds)
{
    return fromMilliseconds(seconds * MillisecondsPerSecond);
}

TimeSpan TimeSpan::fromMinutes(int minutes)
{
    return fromMilliseconds(minutes * MillisecondsPerMinute);
}

TimeSpan TimeSpan::fromHours(int hours)
{
    return fromMilliseconds(hours * MillisecondsPerHour);
}

TimeSpan TimeSpan::fromDays(int days)
{
    return fromMilliseconds(days * MillisecondsPerDay);
}

TimeSpan TimeSpan::absDiff(const QDateTime &t1, const QDateTime &t2)
{
    TimeSpan result;
    if(t1 > t2)
        result = fromMilliseconds(t1.toMSecsSinceEpoch() - t2.toMSecsSinceEpoch());
    else
        result = fromMilliseconds(t2.toMSecsSinceEpoch() - t1.toMSecsSinceEpoch());
    return result;
}

void TimeSpan::toTimeSpec(struct timespec& timespec) const
{
    timespec.tv_sec = totalSeconds();
    timespec.tv_nsec = _milliseconds * 1000000;
}

void TimeSpan::loadFromMilliseconds(uint64_t milliseconds)
{
    _days = milliseconds / MillisecondsPerDay;
    if(_days > 0)
    {
        milliseconds -= _days * MillisecondsPerDay;
    }

    _hours = (int)(milliseconds / MillisecondsPerHour);
    if(_hours > 0)
    {
        milliseconds -= _hours * MillisecondsPerHour;
    }

    _minutes = (int)(milliseconds / MillisecondsPerMinute);
    if(_minutes > 0)
    {
        milliseconds -= _minutes * MillisecondsPerMinute;
    }

    _seconds = (int)(milliseconds / MillisecondsPerSecond);
    if(_seconds > 0)
    {
        milliseconds -= _seconds * MillisecondsPerSecond;
    }

    _milliseconds = milliseconds;
}

QString TimeSpan::toString() const
{
    QString result = _days == 0
            ? QString("%1:%2:%3.%4").
              arg(_hours, 2, 10, QChar('0')).
              arg(_minutes, 2, 10, QChar('0')).
              arg(_seconds, 2, 10, QChar('0')).
              arg(_milliseconds, 3, 10, QChar('0'))
            : QString("%1.:%2:%3:%4.%5").
              arg(_days).
              arg(_hours, 2, 10, QChar('0')).
              arg(_minutes, 2, 10, QChar('0')).
              arg(_seconds, 2, 10, QChar('0')).
              arg(_milliseconds, 3, 10, QChar('0'));
    return result;
}

QString TimeSpan::toAbbreviatedFormat(bool milliseconds)
{
    QString result;
    QTextStream output(&result);

    if (totalMilliseconds() == 0)
        output << "0m";
    else
    {
        if(totalMilliseconds() < 0)
        {
            output << "-";
        }

        double days = totalDays();
        double years = 0;

        if(days > DaysPerYear)
        {
            years = days / DaysPerYear;
            if(totalDays() > DaysPerYear)
            {
                days -= years * DaysPerYear;
            }
        }

        if ((int)years != 0)
            output << (int)years << "y ";
        if ((int)days != 0)
            output << (int)days << "d ";
        if (_hours != 0)
            output << _hours << "h ";
        if (_minutes != 0)
            output << _minutes << "m ";
        if (_seconds != 0)
            output << _seconds << "s ";
        if(milliseconds)
        {
            if (_milliseconds != 0)
                output << _milliseconds << "ms ";
        }
    }
    return result.trimmed();
}


