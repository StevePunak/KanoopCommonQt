#include "timeconstants.h"
#include "timespan2.h"

#include <QTextStream>
#include <QDebug>

const double TimeSpan2::NanosecondsPerMicrosecond       = (double)__NanosecondsPerMicrosecond;
const double TimeSpan2::NanosecondsPerMillisecond       = (double)__NanosecondsPerMillisecond;
const double TimeSpan2::NanosecondsPerSecond            = (double)__NanosecondsPerSecond;
const double TimeSpan2::NanosecondsPerMinute            = (double)__NanosecondsPerMinute;
const double TimeSpan2::NanosecondsPerHour              = (double)__NanosecondsPerHour;
const double TimeSpan2::NanosecondsPerDay               = (double)__NanosecondsPerDay;

const double TimeSpan2::MicrosecondsPerMillisecond    = (double)__MicrosecondsPerMillisecond;
const double TimeSpan2::MicrosecondsPerSecond         = (double)__MicrosecondsPerSecond;
const double TimeSpan2::MicrosecondsPerMinute         = (double)__MicrosecondsPerMinute;
const double TimeSpan2::MicrosecondsPerHour           = (double)__MicrosecondsPerHour;
const double TimeSpan2::MicrosecondsPerDay            = (double)__MicrosecondsPerDay;

const double TimeSpan2::MillisecondsPerSecond         = (double)__MillisecondsPerSecond;
const double TimeSpan2::MillisecondsPerMinute         = (double)__MillisecondsPerMinute;
const double TimeSpan2::MillisecondsPerHour           = (double)__MillisecondsPerHour;
const double TimeSpan2::MillisecondsPerDay            = (double)__MillisecondsPerDay;

const double TimeSpan2::SecondsPerMinute              = (double)__SecondsPerMinute;
const double TimeSpan2::SecondsPerHour                = (double)__SecondsPerHour;
const double TimeSpan2::SecondsPerDay                 = (double)__SecondsPerDay;

const double TimeSpan2::MinutesPerHour                = (double)__MinutesPerHour;
const double TimeSpan2::MinutesPerDay                 = (double)__MinutesPerDay;

const double TimeSpan2::HoursPerDay                   = (double)__HoursPerDay;

const double TimeSpan2::DaysPerYear                   = (double)__DaysPerYear;

TimeSpan2::TimeSpan2(double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds) :
    _nanoseconds(0)
{
    _nanoseconds += days * NanosecondsPerDay;
    _nanoseconds += hours * NanosecondsPerHour;
    _nanoseconds += minutes * NanosecondsPerMinute;
    _nanoseconds += seconds * NanosecondsPerSecond;
    _nanoseconds += milliseconds * NanosecondsPerMillisecond;
    _nanoseconds += microseconds * NanosecondsPerMicrosecond;
    _nanoseconds += nanoseconds;

    qDebug() << _nanoseconds << Qt::endl;
}

TimeSpan2::TimeSpan2(const TimeSpan2& other)
{
    *this = other;
}

TimeSpan2::TimeSpan2(const timespec& other)
{
    *this = other;
}

TimeSpan2& TimeSpan2::operator =(const timespec& other)
{
    _nanoseconds = (uint64_t)(other.tv_sec) * 1000L;
    _nanoseconds += (uint64_t)other.tv_nsec;
    return *this;
}

TimeSpan2& TimeSpan2::operator =(const TimeSpan2& other)
{
    _nanoseconds = other._nanoseconds;
    return *this;
}

double TimeSpan2::totalSeconds() const
{
    double result = (double)_nanoseconds / NanosecondsPerSecond;
    return result;
}

double TimeSpan2::totalMinutes() const
{
    double result = (double)_nanoseconds / NanosecondsPerMinute;
    return result;
}

double TimeSpan2::totalHours() const
{
    double result = (double)_nanoseconds / NanosecondsPerHour;
    return result;
}

double TimeSpan2::totalDays() const
{
    double result = (double)_nanoseconds / NanosecondsPerDay;
    return result;
}

double TimeSpan2::totalMilliseconds() const
{
    double result = (double)_nanoseconds / NanosecondsPerMillisecond;
    return result;
}

double TimeSpan2::totalMicroseconds() const
{
    double result = (double)_nanoseconds / NanosecondsPerMicrosecond;
    return result;
}

double TimeSpan2::totalNanoseconds() const
{
    return (double)_nanoseconds;
}

TimeSpan2 TimeSpan2::operator +(const TimeSpan2& other) const
{
    TimeSpan2 temp;
    temp._nanoseconds = _nanoseconds + other._nanoseconds;
    return temp;
}

TimeSpan2 TimeSpan2::operator -(const TimeSpan2& other) const
{
    TimeSpan2 temp;
    temp._nanoseconds = _nanoseconds - other._nanoseconds;
    return temp;
}

void TimeSpan2::operator +=(const TimeSpan2& other)
{
    *this = *this + other;
}

void TimeSpan2::operator -=(const TimeSpan2& other)
{
    *this = *this - other;
}

bool TimeSpan2::operator ==(const TimeSpan2& other) const
{
    return _nanoseconds == other._nanoseconds;
}

bool TimeSpan2::operator !=(const TimeSpan2& other) const
{
    return _nanoseconds != other._nanoseconds;
}

bool TimeSpan2::operator >(const TimeSpan2& other) const
{
    return _nanoseconds > other._nanoseconds;
}

bool TimeSpan2::operator <(const TimeSpan2& other) const
{
    return _nanoseconds < other._nanoseconds;
}

bool TimeSpan2::operator >=(const TimeSpan2& other) const
{
    return _nanoseconds >= other._nanoseconds;
}

bool TimeSpan2::operator <=(const TimeSpan2& other) const
{
    return _nanoseconds <= other._nanoseconds;
}

TimeSpan2 TimeSpan2::addNanoseconds(double nanoseconds) const
{
    return fromNanoseconds(_nanoseconds + nanoseconds);
}

TimeSpan2 TimeSpan2::addMicroseconds(double microseconds) const
{
    return fromNanoseconds(_nanoseconds + (microseconds * NanosecondsPerMicrosecond));
}

TimeSpan2 TimeSpan2::addMilliseconds(double milliseconds) const
{
    return fromNanoseconds(_nanoseconds + (milliseconds * NanosecondsPerMillisecond));
}

TimeSpan2 TimeSpan2::addSeconds(double seconds) const
{
    return fromNanoseconds(_nanoseconds + (seconds * NanosecondsPerSecond));
}

TimeSpan2 TimeSpan2::addMinutes(double minutes) const
{
    return fromNanoseconds(_nanoseconds + (minutes * NanosecondsPerMinute));
}

TimeSpan2 TimeSpan2::addHours(double hours) const
{
    return fromNanoseconds(_nanoseconds + (hours * NanosecondsPerHour));
}

TimeSpan2 TimeSpan2::addDays(double days) const
{
    return fromNanoseconds(_nanoseconds + (days * NanosecondsPerDay));
}

qint64 TimeSpan2::days() const
{
    qint64 d = _nanoseconds / (qint64)NanosecondsPerDay;
    return d;
}

qint64 TimeSpan2::hours() const
{
    qint64 value = _nanoseconds;
    value -= days() * NanosecondsPerDay;
    value /= (qint64)NanosecondsPerHour;
    return value;
}

qint64 TimeSpan2::minutes() const
{
    qint64 value = _nanoseconds;
    value -= days() * NanosecondsPerDay;
    value -= hours() * NanosecondsPerHour;
    value /=  (qint64)NanosecondsPerMinute;
    return value;
}

qint64 TimeSpan2::seconds() const
{
    qint64 value = _nanoseconds;
    value -= days() * NanosecondsPerDay;
    value -= hours() * NanosecondsPerHour;
    value -= minutes() * NanosecondsPerMinute;
    value /=  (qint64)NanosecondsPerSecond;
    return value;
}

qint64 TimeSpan2::milliseconds() const
{
    qint64 value = _nanoseconds;;
    value -= days() * NanosecondsPerDay;
    value -= hours() * NanosecondsPerHour;
    value -= minutes() * NanosecondsPerMinute;
    value -= seconds() * NanosecondsPerSecond;
    value /=  (qint64)NanosecondsPerMillisecond;
    return value;
}

qint64 TimeSpan2::microseconds() const
{
    qint64 value = _nanoseconds;
    value -= days() * NanosecondsPerDay;
    value -= hours() * NanosecondsPerHour;
    value -= minutes() * NanosecondsPerMinute;
    value -= seconds() * NanosecondsPerSecond;
    value -= milliseconds() * NanosecondsPerMillisecond;
    value /= (qint64)NanosecondsPerMicrosecond;
    return value;
}

qint64 TimeSpan2::nanoseconds() const
{
    qint64 value = _nanoseconds;
    value -= days() * NanosecondsPerDay;
    value -= hours() * NanosecondsPerHour;
    value -= minutes() * NanosecondsPerMinute;
    value -= seconds() * NanosecondsPerSecond;
    value -= milliseconds() * NanosecondsPerMillisecond;
    value -= microseconds() * NanosecondsPerMicrosecond;
    return value;
}

TimeSpan2 TimeSpan2::fromNanoseconds(double nanoseconds)
{
    TimeSpan2 ts(0, 0, 0, 0, 0, 0, nanoseconds);
    return ts;
}

TimeSpan2 TimeSpan2::fromMicroseconds(double microseconds)
{
    TimeSpan2 ts(0, 0, 0, 0, 0, microseconds, 0);
    return ts;
}

TimeSpan2 TimeSpan2::fromMilliseconds(double milliseconds)
{
    TimeSpan2 ts(0, 0, 0, 0, milliseconds);
    return ts;
}

TimeSpan2 TimeSpan2::fromSeconds(double seconds)
{
    TimeSpan2 ts(0, 0, 0, seconds);
    return ts;
}

TimeSpan2 TimeSpan2::fromMinutes(double minutes)
{
    TimeSpan2 ts(0, 0, minutes, 0);
    return ts;
}

TimeSpan2 TimeSpan2::fromHours(double hours)
{
    TimeSpan2 ts(0, hours, 0, 0);
    return ts;
}

TimeSpan2 TimeSpan2::fromDays(double days)
{
    TimeSpan2 ts(days, 0, 0, 0);
    return ts;
}

TimeSpan2 TimeSpan2::fromString(const QString &timeString)
{
    if(timeString.contains('d') || timeString.contains('h') || timeString.contains('m') || timeString.contains('s'))
        return parseAbbreviatedString(timeString);
    else if(timeString.contains(':'))
        return parseColonDelimitedString(timeString);
    else if(timeString.length() > 0 && timeString[0].isDigit())
    {
        return TimeSpan2::fromMilliseconds(timeString.toInt());
    }
    return TimeSpan2::zero();
}

TimeSpan2 TimeSpan2::parseAbbreviatedString(const QString &timeString)
{
    QString remaining = timeString.trimmed();
    TimeSpan2 result = TimeSpan2::zero();

    int days = parseIntToToken(remaining, "d");
    if(days >= 0)
        result = result + TimeSpan2::fromDays(days);

    int hours = parseIntToToken(remaining, "h");
    if(hours >= 0)
        result = result + TimeSpan2::fromHours(hours);

    int minutes = parseIntToToken(remaining, "m");
    if(minutes >= 0)
        result = result + TimeSpan2::fromMinutes(minutes);

    int seconds = parseIntToToken(remaining, "s");
    if(seconds >= 0)
        result = result + TimeSpan2::fromSeconds(seconds);

    int milliseconds = parseIntToToken(remaining, "ms");
    if(milliseconds >= 0)
        result = result + TimeSpan2::fromMilliseconds(milliseconds);
    return result;
}

TimeSpan2 TimeSpan2::parseColonDelimitedString(const QString &timeString)
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
        ms = (int)((s - seconds) * 1000);
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

    TimeSpan2 result = TimeSpan2(days, hours, minutes, seconds, ms);
    return result;
}

int TimeSpan2::parseIntToToken(QString &remaining, const QString& token)
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

TimeSpan2 TimeSpan2::absDiff(const QDateTime &t1, const QDateTime &t2)
{
    TimeSpan2 result;
    if(t1 > t2)
        result = fromMilliseconds(t1.toMSecsSinceEpoch() - t2.toMSecsSinceEpoch());
    else
        result = fromMilliseconds(t2.toMSecsSinceEpoch() - t1.toMSecsSinceEpoch());
    return result;
}

void TimeSpan2::toTimeSpec(struct timespec& timespec) const
{
    timespec.tv_sec = (long)totalSeconds();
    timespec.tv_nsec = _nanoseconds % (qint64)NanosecondsPerSecond;
}

QString TimeSpan2::toString() const
{
    QString result = days() == 0
            ? QString("%1:%2:%3.%4").
              arg(hours(), 2, 10, QChar('0')).
              arg(minutes(), 2, 10, QChar('0')).
              arg(seconds(), 2, 10, QChar('0')).
              arg(milliseconds(), 3, 10, QChar('0'))
            : QString("%1.:%2:%3:%4.%5").
              arg(days()).
              arg(hours(), 2, 10, QChar('0')).
              arg(minutes(), 2, 10, QChar('0')).
              arg(seconds(), 2, 10, QChar('0')).
              arg(milliseconds(), 3, 10, QChar('0'));
    return result;
}

QString TimeSpan2::toAbbreviatedFormat(bool showMilliseconds) const
{
    QString result;
    QTextStream output(&result);

    if (_nanoseconds == 0)
        output << "0m";
    else
    {
        if(_nanoseconds < 0)
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
        if (hours() != 0)
            output << hours() << "h ";
        if (minutes() != 0)
            output << minutes() << "m ";
        if (seconds() != 0)
            output << seconds() << "s ";
        if(showMilliseconds)
        {
            if(milliseconds() != 0)
                output << milliseconds() << "ms ";
        }
    }
    return result.trimmed();
}

QString TimeSpan2::toDumpString() const
{
    static const int COL1_WIDTH = 32;

    QString result;
    QTextStream output(&result);

    output << QString("Days:").leftJustified(COL1_WIDTH) << days() << Qt::endl;
    output << QString("Hours:").leftJustified(COL1_WIDTH) << hours() << Qt::endl;
    output << QString("Minutes:").leftJustified(COL1_WIDTH) << minutes() << Qt::endl;
    output << QString("Seconds:").leftJustified(COL1_WIDTH) << seconds() << Qt::endl;
    output << QString("Milliseconds:").leftJustified(COL1_WIDTH) << milliseconds() << Qt::endl;
    output << QString("Microseconds:").leftJustified(COL1_WIDTH) << microseconds() << Qt::endl;
    output << QString("Nanoseconds:").leftJustified(COL1_WIDTH) << nanoseconds() << Qt::endl;

    return result;
}



