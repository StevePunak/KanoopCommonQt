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
 *  Stephen Punak, December 27 2021
 */
#ifndef TIMESPAN2_H
#define TIMESPAN2_H
#include <QString>
#include <QObject>
#include <QDateTime>
#include <time.h>
#include <sys/time.h>
#include "kanoopcommon.h"

class KANOOP_EXPORT TimeSpan2
{
public:
  TimeSpan2() :
    _nanoseconds(0) {}

  TimeSpan2(double days, double hours, double minutes, double seconds, double milliseconds = 0, double microseconds = 0, double nanoseconds = 0);

  TimeSpan2(const TimeSpan2& other);
  TimeSpan2(const timespec& other);

  TimeSpan2& operator=(const TimeSpan2& other);
  TimeSpan2& operator=(const timespec& other);

  TimeSpan2 operator+(const TimeSpan2& other) const;
  TimeSpan2 operator-(const TimeSpan2& other) const;
  TimeSpan2 operator*(const TimeSpan2& other) const;
  TimeSpan2 operator*(const int& value) const;
  TimeSpan2 operator/(const TimeSpan2& other) const;
  TimeSpan2 operator/(const int& value) const;
  void operator+=(const TimeSpan2& other);
  void operator-=(const TimeSpan2& other);
  void operator*=(const TimeSpan2& other);
  void operator/=(const TimeSpan2& other);

  bool operator==(const TimeSpan2& other) const;
  bool operator!=(const TimeSpan2& other) const;
  bool operator>(const TimeSpan2& other) const;
  bool operator<(const TimeSpan2& other) const;
  bool operator>=(const TimeSpan2& other) const;
  bool operator<=(const TimeSpan2& other) const;

  qint64 days() const;
  qint64 hours() const;
  qint64 minutes() const;
  qint64 seconds() const;
  qint64 milliseconds() const;
  qint64 microseconds() const;
  qint64 nanoseconds() const;

  double totalSeconds() const;
  double totalMinutes() const;
  double totalHours() const;
  double totalDays() const;
  double totalMilliseconds() const;
  double totalMicroseconds() const;
  double totalNanoseconds() const;

  void toTimeSpec(struct timespec& timespec) const;
  QString toString() const;
  QString toAbbreviatedFormat(bool showMilliseconds = false) const;
  QString toDumpString() const;

  TimeSpan2 addNanoseconds(double nanoseconds) const;
  TimeSpan2 addMicroseconds(double microseconds) const;
  TimeSpan2 addMilliseconds(double milliseconds) const;
  TimeSpan2 addSeconds(double seconds) const;
  TimeSpan2 addMinutes(double minutes) const;
  TimeSpan2 addHours(double hours) const;
  TimeSpan2 addDays(double days) const;

  static TimeSpan2 fromNanoseconds(double nanoseconds);
  static TimeSpan2 fromMicroseconds(double microseconds);
  static TimeSpan2 fromMilliseconds(double milliseconds);
  static TimeSpan2 fromSeconds(double seconds);
  static TimeSpan2 fromMinutes(double minutes);
  static TimeSpan2 fromHours(double hours);
  static TimeSpan2 fromDays(double days);
  static TimeSpan2 fromString(const QString& timeString);
  static TimeSpan2 zero() { return TimeSpan2::fromSeconds(0); }
  static TimeSpan2 absDiff(const QDateTime& t1, const QDateTime& t2);

  static TimeSpan2 max(const TimeSpan2& t1, const TimeSpan2& t2) { return t1 > t2 ? t1 : t2; }
  static TimeSpan2 min(const TimeSpan2& t1, const TimeSpan2& t2) { return t1 < t2 ? t1 : t2; }

private:
  static TimeSpan2 parseAbbreviatedString(const QString& timeString);
  static TimeSpan2 parseColonDelimitedString(const QString& timeString);
  static int parseIntToToken(QString &remaining, const QString &until);

  qint64    _nanoseconds;

// Constants
    static const double NanosecondsPerMicrosecond;
    static const double NanosecondsPerMillisecond;
    static const double NanosecondsPerSecond;
    static const double NanosecondsPerMinute;
    static const double NanosecondsPerHour;
    static const double NanosecondsPerDay;

    static const double MicrosecondsPerMillisecond;
    static const double MicrosecondsPerSecond;
    static const double MicrosecondsPerMinute;
    static const double MicrosecondsPerHour;
    static const double MicrosecondsPerDay;

    static const double MillisecondsPerSecond;
    static const double MillisecondsPerMinute;
    static const double MillisecondsPerHour;
    static const double MillisecondsPerDay;

    static const double SecondsPerMinute;
    static const double SecondsPerHour;
    static const double SecondsPerDay;

    static const double MinutesPerHour;
    static const double MinutesPerDay;

    static const double HoursPerDay;

    static const double   DaysPerYear;
};

Q_DECLARE_METATYPE(TimeSpan2)

#endif // TIMESPAN2_H
