#include "timeconstants.h"
#include "timespan.h"

#include <QTextStream>

const int64_t TimeSpan::MicrosecondsPerMillisecond    = __MicrosecondsPerMillisecond;
const int64_t TimeSpan::MicrosecondsPerSecond         = __MicrosecondsPerSecond;
const int64_t TimeSpan::MicrosecondsPerMinute         = __MicrosecondsPerMinute;
const int64_t TimeSpan::MicrosecondsPerHour           = __MicrosecondsPerHour;
const int64_t TimeSpan::MicrosecondsPerDay            = __MicrosecondsPerDay;

const int64_t TimeSpan::MillisecondsPerSecond         = __MillisecondsPerSecond;
const int64_t TimeSpan::MillisecondsPerMinute         = __MillisecondsPerMinute;
const int64_t TimeSpan::MillisecondsPerHour           = __MillisecondsPerHour;
const int64_t TimeSpan::MillisecondsPerDay            = __MillisecondsPerDay;

const int64_t TimeSpan::SecondsPerMinute              = __SecondsPerMinute;
const int64_t TimeSpan::SecondsPerHour                = __SecondsPerHour;
const int64_t TimeSpan::SecondsPerDay                 = __SecondsPerDay;

const int64_t TimeSpan::MinutesPerHour                = __MinutesPerHour;
const int64_t TimeSpan::MinutesPerDay                 = __MinutesPerDay;

const int64_t TimeSpan::HoursPerDay                   = __HoursPerDay;

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

TimeSpan TimeSpan::fromMilliseconds(int64_t milliseconds)
{
    TimeSpan ts;
    ts.loadFromMilliseconds(milliseconds);
    return ts;
}

TimeSpan TimeSpan::fromSeconds(int64_t seconds)
{
    return fromMilliseconds(seconds * MillisecondsPerSecond);
}

TimeSpan TimeSpan::fromMinutes(int64_t minutes)
{
    return fromMilliseconds(minutes * MillisecondsPerMinute);
}

TimeSpan TimeSpan::fromHours(int64_t hours)
{
    return fromMilliseconds(hours * MillisecondsPerHour);
}

TimeSpan TimeSpan::fromDays(int64_t days)
{
    return fromMilliseconds(days * MillisecondsPerDay);
}

TimeSpan TimeSpan::fromString(const QString &timeString)
{
    if(timeString.contains('d') || timeString.contains('h') || timeString.contains('m') || timeString.contains('s'))
        return parseAbbreviatedString(timeString);
    else if(timeString.contains(':'))
        return parseColonDelimitedString(timeString);
    else if(timeString.length() > 0 && timeString[0].isDigit())
    {
        return TimeSpan::fromMilliseconds(timeString.toInt());
    }
    return TimeSpan::zero();
}

TimeSpan TimeSpan::parseAbbreviatedString(const QString &timeString)
{
    QString remaining = timeString.trimmed();
    TimeSpan result = TimeSpan::zero();

    int days = parseIntToToken(remaining, "d");
    if(days >= 0)
        result = result + TimeSpan::fromDays(days);

    int hours = parseIntToToken(remaining, "h");
    if(hours >= 0)
        result = result + TimeSpan::fromHours(hours);

    int minutes = parseIntToToken(remaining, "m");
    if(minutes >= 0)
        result = result + TimeSpan::fromMinutes(minutes);

    int seconds = parseIntToToken(remaining, "s");
    if(seconds >= 0)
        result = result + TimeSpan::fromSeconds(seconds);

    int milliseconds = parseIntToToken(remaining, "ms");
    if(milliseconds >= 0)
        result = result + TimeSpan::fromMilliseconds(milliseconds);
    return result;
}

TimeSpan TimeSpan::parseColonDelimitedString(const QString &timeString)
{
    QStringList parts = timeString.split(':');
    int days = 0;
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    int ms = 0;
    if(parts.length() > 0)
    {
        double s = parts.last().toDouble();
        seconds = (int)s;
        ms = (int)(s - seconds);
        parts.removeLast();
    }
    if(parts.length() > 0)
    {
        minutes = parts.last().toInt();
        parts.removeLast();
    }
    if(parts.length() > 0)
    {
        hours = parts.last().toInt();
        parts.removeLast();
    }
    if(parts.length() > 0)
    {
        days = parts.last().toInt();
    }

    TimeSpan result = TimeSpan(days, hours, minutes, seconds, ms);
    return result;
}

int TimeSpan::parseIntToToken(QString &remaining, const QString& token)
{
    int result = -1;
    int index = remaining.indexOf(token);

    // make sure the token has no adjoining alpha characters
    if(index > 0)
    {
        int tokenSizeAtLocation = 0;
        for(;index + tokenSizeAtLocation < remaining.length() && remaining[index + tokenSizeAtLocation].isLetter();tokenSizeAtLocation++);
        if(tokenSizeAtLocation == token.length())
        {
            QString part = remaining.mid(0, index);
            result = part.toInt();
            remaining = part.length() + index < remaining.length()
                    ? remaining.mid(index + part.length()).trimmed()
                    : QString();
        }
    }
    return result;
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

void TimeSpan::loadFromMilliseconds(int64_t milliseconds)
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

QString TimeSpan::toAbbreviatedFormat(bool milliseconds) const
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


