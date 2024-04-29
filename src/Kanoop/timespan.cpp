/**
 *  TimeSpan
 *
 *  This class represents a span of time with nanosecond resolution, and provides
 *  an interface that is very similar to the .NET TimeSpan class.
 *
 *  Operators are supplied for comparison and assignment.
 *
 *  Conversions are supplied to convert between timespec and TimeSpan.
 *
 *  Use the absDiff() static method to obtain the difference between two
 *  QDateTime objects.
 *
 *  NOTE:
 *  The underlying representation is a signed 64-bit integer value of nanoseconds.
 *  Therefore, the largest possible value is 9,223,372,036,854,775,807 giving a
 *  maximum range of approximately 292 years.
 *
 *  This was re-written December 2021 to have nanosecond resolution. The interface
 *  is compatible with the previous TimeSpan object.
 *
 *  Stephen Punak, December 27 2021
 */
#include "Kanoop/timeconstants.h"
#include "Kanoop/timespan.h"

#include <QTextStream>
#include <QDebug>

const double TimeSpan::NanosecondsPerMicrosecond       = (double)__NanosecondsPerMicrosecond;
const double TimeSpan::NanosecondsPerMillisecond       = (double)__NanosecondsPerMillisecond;
const double TimeSpan::NanosecondsPerSecond            = (double)__NanosecondsPerSecond;
const double TimeSpan::NanosecondsPerMinute            = (double)__NanosecondsPerMinute;
const double TimeSpan::NanosecondsPerHour              = (double)__NanosecondsPerHour;
const double TimeSpan::NanosecondsPerDay               = (double)__NanosecondsPerDay;

const double TimeSpan::MicrosecondsPerMillisecond    = (double)__MicrosecondsPerMillisecond;
const double TimeSpan::MicrosecondsPerSecond         = (double)__MicrosecondsPerSecond;
const double TimeSpan::MicrosecondsPerMinute         = (double)__MicrosecondsPerMinute;
const double TimeSpan::MicrosecondsPerHour           = (double)__MicrosecondsPerHour;
const double TimeSpan::MicrosecondsPerDay            = (double)__MicrosecondsPerDay;

const double TimeSpan::MillisecondsPerSecond         = (double)__MillisecondsPerSecond;
const double TimeSpan::MillisecondsPerMinute         = (double)__MillisecondsPerMinute;
const double TimeSpan::MillisecondsPerHour           = (double)__MillisecondsPerHour;
const double TimeSpan::MillisecondsPerDay            = (double)__MillisecondsPerDay;

const double TimeSpan::SecondsPerMinute              = (double)__SecondsPerMinute;
const double TimeSpan::SecondsPerHour                = (double)__SecondsPerHour;
const double TimeSpan::SecondsPerDay                 = (double)__SecondsPerDay;

const double TimeSpan::MinutesPerHour                = (double)__MinutesPerHour;
const double TimeSpan::MinutesPerDay                 = (double)__MinutesPerDay;

const double TimeSpan::HoursPerDay                   = (double)__HoursPerDay;

const double TimeSpan::DaysPerYear                   = (double)__DaysPerYear;

TimeSpan::TimeSpan(double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds) :
    _nanoseconds(0)
{
    _nanoseconds += days * NanosecondsPerDay;
    _nanoseconds += hours * NanosecondsPerHour;
    _nanoseconds += minutes * NanosecondsPerMinute;
    _nanoseconds += seconds * NanosecondsPerSecond;
    _nanoseconds += milliseconds * NanosecondsPerMillisecond;
    _nanoseconds += microseconds * NanosecondsPerMicrosecond;
    _nanoseconds += nanoseconds;
}

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
    _nanoseconds = (uint64_t)(other.tv_sec) * 1000000000L;
    _nanoseconds += (uint64_t)other.tv_nsec;
    return *this;
}

TimeSpan& TimeSpan::operator =(const TimeSpan& other)
{
    _nanoseconds = other._nanoseconds;
    return *this;
}

double TimeSpan::totalSeconds() const
{
    double result = (double)_nanoseconds / NanosecondsPerSecond;
    return result;
}

double TimeSpan::totalMinutes() const
{
    double result = (double)_nanoseconds / NanosecondsPerMinute;
    return result;
}

double TimeSpan::totalHours() const
{
    double result = (double)_nanoseconds / NanosecondsPerHour;
    return result;
}

double TimeSpan::totalDays() const
{
    double result = (double)_nanoseconds / NanosecondsPerDay;
    return result;
}

double TimeSpan::totalMilliseconds() const
{
    double result = (double)_nanoseconds / NanosecondsPerMillisecond;
    return result;
}

double TimeSpan::totalMicroseconds() const
{
    double result = (double)_nanoseconds / NanosecondsPerMicrosecond;
    return result;
}

double TimeSpan::totalNanoseconds() const
{
    return (double)_nanoseconds;
}

TimeSpan TimeSpan::operator +(const TimeSpan& other) const
{
    TimeSpan temp;
    temp._nanoseconds = _nanoseconds + other._nanoseconds;
    return temp;
}

TimeSpan TimeSpan::operator -(const TimeSpan& other) const
{
    TimeSpan temp;
    temp._nanoseconds = _nanoseconds - other._nanoseconds;
    return temp;
}

TimeSpan TimeSpan::operator*(const TimeSpan &other) const
{
    TimeSpan temp;
    temp._nanoseconds = _nanoseconds * other._nanoseconds;
    return temp;
}

TimeSpan TimeSpan::operator*(double value) const
{
    TimeSpan temp;
    temp._nanoseconds = _nanoseconds * value;
    return temp;
}

TimeSpan TimeSpan::operator/(const TimeSpan &other) const
{
    TimeSpan temp;
    temp._nanoseconds = _nanoseconds / other._nanoseconds;
    return temp;
}

TimeSpan TimeSpan::operator/(double value) const
{
    TimeSpan temp;
    temp._nanoseconds = _nanoseconds / value;
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

void TimeSpan::operator*=(const TimeSpan &other)
{
    *this = *this * other;
}

void TimeSpan::operator/=(const TimeSpan &other)
{
    *this = *this / other;
}

bool TimeSpan::operator ==(const TimeSpan& other) const
{
    return _nanoseconds == other._nanoseconds;
}

bool TimeSpan::operator !=(const TimeSpan& other) const
{
    return _nanoseconds != other._nanoseconds;
}

bool TimeSpan::operator >(const TimeSpan& other) const
{
    return _nanoseconds > other._nanoseconds;
}

bool TimeSpan::operator <(const TimeSpan& other) const
{
    return _nanoseconds < other._nanoseconds;
}

bool TimeSpan::operator >=(const TimeSpan& other) const
{
    return _nanoseconds >= other._nanoseconds;
}

bool TimeSpan::operator <=(const TimeSpan& other) const
{
    return _nanoseconds <= other._nanoseconds;
}

void TimeSpan::operator*=(double value)
{
    _nanoseconds *= value;
}

void TimeSpan::operator/=(double value)
{
    _nanoseconds /= value;
}

TimeSpan TimeSpan::addNanoseconds(double nanoseconds) const
{
    return fromNanoseconds(_nanoseconds + nanoseconds);
}

TimeSpan TimeSpan::addMicroseconds(double microseconds) const
{
    return fromNanoseconds(_nanoseconds + (microseconds * NanosecondsPerMicrosecond));
}

TimeSpan TimeSpan::addMilliseconds(double milliseconds) const
{
    return fromNanoseconds(_nanoseconds + (milliseconds * NanosecondsPerMillisecond));
}

TimeSpan TimeSpan::addSeconds(double seconds) const
{
    return fromNanoseconds(_nanoseconds + (seconds * NanosecondsPerSecond));
}

TimeSpan TimeSpan::addMinutes(double minutes) const
{
    return fromNanoseconds(_nanoseconds + (minutes * NanosecondsPerMinute));
}

TimeSpan TimeSpan::addHours(double hours) const
{
    return fromNanoseconds(_nanoseconds + (hours * NanosecondsPerHour));
}

TimeSpan TimeSpan::addDays(double days) const
{
    return fromNanoseconds(_nanoseconds + (days * NanosecondsPerDay));
}

qint64 TimeSpan::days() const
{
    qint64 d = _nanoseconds / (qint64)NanosecondsPerDay;
    return d;
}

qint64 TimeSpan::hours() const
{
    qint64 value = _nanoseconds;
    value -= days() * NanosecondsPerDay;
    value /= (qint64)NanosecondsPerHour;
    return value;
}

qint64 TimeSpan::minutes() const
{
    qint64 value = _nanoseconds;
    value -= days() * NanosecondsPerDay;
    value -= hours() * NanosecondsPerHour;
    value /=  (qint64)NanosecondsPerMinute;
    return value;
}

qint64 TimeSpan::seconds() const
{
    qint64 value = _nanoseconds;
    value -= days() * NanosecondsPerDay;
    value -= hours() * NanosecondsPerHour;
    value -= minutes() * NanosecondsPerMinute;
    value /=  (qint64)NanosecondsPerSecond;
    return value;
}

qint64 TimeSpan::milliseconds() const
{
    qint64 value = _nanoseconds;;
    value -= days() * NanosecondsPerDay;
    value -= hours() * NanosecondsPerHour;
    value -= minutes() * NanosecondsPerMinute;
    value -= seconds() * NanosecondsPerSecond;
    value /=  (qint64)NanosecondsPerMillisecond;
    return value;
}

qint64 TimeSpan::microseconds() const
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

qint64 TimeSpan::nanoseconds() const
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

TimeSpan TimeSpan::fromNanoseconds(double nanoseconds)
{
    TimeSpan ts(0, 0, 0, 0, 0, 0, nanoseconds);
    return ts;
}

TimeSpan TimeSpan::fromMicroseconds(double microseconds)
{
    TimeSpan ts(0, 0, 0, 0, 0, microseconds, 0);
    return ts;
}

TimeSpan TimeSpan::fromMilliseconds(double milliseconds)
{
    TimeSpan ts(0, 0, 0, 0, milliseconds);
    return ts;
}

TimeSpan TimeSpan::fromSeconds(double seconds)
{
    TimeSpan ts(0, 0, 0, seconds);
    return ts;
}

TimeSpan TimeSpan::fromMinutes(double minutes)
{
    TimeSpan ts(0, 0, minutes, 0);
    return ts;
}

TimeSpan TimeSpan::fromHours(double hours)
{
    TimeSpan ts(0, hours, 0, 0);
    return ts;
}

TimeSpan TimeSpan::fromDays(double days)
{
    TimeSpan ts(days, 0, 0, 0);
    return ts;
}

TimeSpan TimeSpan::fromString(const QString &timeString)
{
    QStringList tokens = getTokens(timeString);

    if(tokens.contains("d") || tokens.contains("h") || tokens.contains("m") || tokens.contains("s"))
        return parseAbbreviatedString(timeString);
    else if(timeString.contains(':'))
        return parseColonDelimitedString(timeString);
    else if(timeString.length() > 0 && timeString[0].isDigit())
    {
        if(tokens.contains("us")) {
            return parseMicrosecondString(timeString);
        }
        else if(tokens.contains("ms")) {
            return parseMillisecondString(timeString);
        }
        else {
            return TimeSpan::fromMilliseconds(timeString.toInt());
        }
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
        // This rigamorole help with double lack of precision (e.g.: 1.003 will normally parse to 1.0029999999)
        s -= seconds;
        s =  qRound(s * 1000);
        ms = s;
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

TimeSpan TimeSpan::parseMicrosecondString(const QString &timeString)
{
    TimeSpan result;
    int index = timeString.indexOf("us");
    if(index > 0) {
        QString trimmed = timeString.left(index).trimmed();
        double us = trimmed.toDouble();
        result = TimeSpan::fromMicroseconds(us);
    }
    return result;
}

TimeSpan TimeSpan::parseMillisecondString(const QString &timeString)
{
    TimeSpan result;
    int index = timeString.indexOf("ms");
    if(index > 0) {
        QString trimmed = timeString.left(index).trimmed();
        double ms = trimmed.toDouble();
        result = TimeSpan::fromMilliseconds(ms);
    }
    return result;

}

QStringList TimeSpan::getTokens(const QString& timeString)
{
    QStringList result;
    QString token;
    for(int i = 0;i < timeString.length();i++) {
        QChar c = timeString.at(i);
        if(c.isLetter()) {
            token.append(c);
        }
        else if(token.length() > 0) {
            result.append(token);
            token.clear();
        }
    }
    if(token.length() > 0) {
        result.append(token);
    }
    return result;
}

int TimeSpan::parseIntToToken(QString &remaining, const QString& token)
{
    int result = -1;
    int index = remaining.indexOf(token);

    // make sure the token has no adjoining alpha characters
    if(index > 0 && remaining.at(index -1).isLetter() == false)
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

TimeSpan TimeSpan::diff(const QDateTime &now, const QDateTime &then)
{
    TimeSpan result = fromMilliseconds(then.toMSecsSinceEpoch() - now.toMSecsSinceEpoch());
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
    timespec.tv_sec = (long)totalSeconds();
    timespec.tv_nsec = _nanoseconds % (qint64)NanosecondsPerSecond;
}

QString TimeSpan::toString() const
{
    QString result;
    if(qAbs(_nanoseconds) > 1000000)
    {
        result = days() == 0
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
    }
    else
    {
        double microseconds = (double)_nanoseconds / (double)1000;
        result = QString("%1us").arg(microseconds, 0, 'f', 3);
    }
    return result;
}

QString TimeSpan::toAbbreviatedFormat(bool showMilliseconds) const
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

        // this takes care of negative values
        TimeSpan ts;
        ts._nanoseconds = qAbs(_nanoseconds);

        double days = ts.totalDays();
        double years = 0;

        if(days > DaysPerYear)
        {
            years = days / DaysPerYear;
            if(ts.totalDays() > DaysPerYear)
            {
                days -= (int)years * DaysPerYear;
            }
        }

        if ((int)years != 0)
            output << (int)years << "y ";
        if ((int)days != 0)
            output << (int)days << "d ";
        if (ts.hours() != 0)
            output << ts.hours() << "h ";
        if (ts.minutes() != 0)
            output << ts.minutes() << "m ";
        if (ts.seconds() != 0)
            output << ts.seconds() << "s ";
        if(showMilliseconds)
        {
            if(ts.milliseconds() != 0)
                output << ts.milliseconds() << "ms ";
        }
    }
    return result.trimmed();
}

QString TimeSpan::toDumpString() const
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
