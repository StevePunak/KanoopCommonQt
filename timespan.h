#ifndef TIMESPAN_H_
#define TIMESPAN_H_

#include <QString>
#include <QObject>
#include <QDateTime>
#include <time.h>
#include <sys/time.h>
#include "kanoopcommon.h"

using namespace std;

class KANOOP_EXPORT TimeSpan
{
  friend class DateTime;
public:
  TimeSpan() :
    _days(0),
    _hours(0),
    _minutes(0),
    _seconds(0),
    _milliseconds(0) {}

  TimeSpan(int days, int hours, int minutes, int seconds) :
    _days(days),
    _hours(hours),
    _minutes(minutes),
    _seconds(seconds),
    _milliseconds(0) {}

  TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds) :
    _days(days),
    _hours(hours),
    _minutes(minutes),
    _seconds(seconds),
    _milliseconds(milliseconds) {}

  TimeSpan(const TimeSpan& other);
  TimeSpan(const timespec& other);

  TimeSpan& operator=(const TimeSpan& other);
  TimeSpan& operator=(const timespec& other);

  TimeSpan operator+(const TimeSpan& other) const;
  TimeSpan operator-(const TimeSpan& other) const;
  void operator+=(const TimeSpan& other);
  void operator-=(const TimeSpan& other);

  bool operator==(const TimeSpan& other) const;
  bool operator!=(const TimeSpan& other) const;
  bool operator>(const TimeSpan& other) const;
  bool operator<(const TimeSpan& other) const;
  bool operator>=(const TimeSpan& other) const;
  bool operator<=(const TimeSpan& other) const;

  int days() const { return _days; }
  int hours() const { return _hours; }
  int minutes() const { return _minutes; }
  int seconds() const { return _seconds; }
  int milliseconds() const { return _milliseconds; }
  int microseconds() const { return 0; }
  int nanoseconds() const { return 0; }

  double totalSeconds() const;
  double totalMinutes() const;
  double totalHours() const;
  double totalDays() const;
  double totalMilliseconds() const;
  double totalMicroseconds() const;
  double totalNanoseconds() const;

  void toTimeSpec(struct timespec& timespec) const;
  QString toString() const;
  QString toAbbreviatedFormat(bool milliseconds = false) const;

  static TimeSpan fromMilliseconds(int64_t milliseconds);
  static TimeSpan fromSeconds(int64_t seconds);
  static TimeSpan fromMinutes(int64_t minutes);
  static TimeSpan fromHours(int64_t hours);
  static TimeSpan fromDays(int64_t days);
  static TimeSpan fromString(const QString& timeString);
  static TimeSpan zero() { return TimeSpan::fromSeconds(0); }
  static TimeSpan absDiff(const QDateTime& t1, const QDateTime& t2);

  static TimeSpan max(const TimeSpan& t1, const TimeSpan& t2) { return t1 > t2 ? t1 : t2; }
  static TimeSpan min(const TimeSpan& t1, const TimeSpan& t2) { return t1 < t2 ? t1 : t2; }

private:
  void loadFromMilliseconds(int64_t milliseconds);
  static TimeSpan parseAbbreviatedString(const QString& timeString);
  static TimeSpan parseColonDelimitedString(const QString& timeString);
  static int parseIntToToken(QString &remaining, const QString &until);

  int      _days;
  int      _hours;
  int      _minutes;
  int      _seconds;
  int      _milliseconds;

// Constants
  static const int64_t MicrosecondsPerMillisecond;
  static const int64_t MicrosecondsPerSecond;
  static const int64_t MicrosecondsPerMinute;
  static const int64_t MicrosecondsPerHour;
  static const int64_t MicrosecondsPerDay;

  static const int64_t MillisecondsPerSecond;
  static const int64_t MillisecondsPerMinute;
  static const int64_t MillisecondsPerHour;
  static const int64_t MillisecondsPerDay;

  static const int64_t SecondsPerMinute;
  static const int64_t SecondsPerHour;
  static const int64_t SecondsPerDay;

  static const int64_t MinutesPerHour;
  static const int64_t MinutesPerDay;

  static const int64_t HoursPerDay;

  static const double   DaysPerYear;
};

Q_DECLARE_METATYPE(TimeSpan)

#endif /* TIMESPAN_H_ */

