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
#ifndef TIMESPAN_H_
#define TIMESPAN_H_
#include <QString>
#include <QObject>
#include <QDateTime>
#include <time.h>
#include <sys/time.h>
#include "kanoopcommon.h"

class KANOOP_EXPORT TimeSpan
{
public:
  TimeSpan() :
    _nanoseconds(0) {}

  TimeSpan(double days, double hours, double minutes, double seconds, double milliseconds = 0, double microseconds = 0, double nanoseconds = 0);

  TimeSpan(const TimeSpan& other);
  TimeSpan(const timespec& other);

  TimeSpan& operator=(const TimeSpan& other);
  TimeSpan& operator=(const timespec& other);

  TimeSpan operator+(const TimeSpan& other) const;
  TimeSpan operator-(const TimeSpan& other) const;
  TimeSpan operator*(const TimeSpan& other) const;
  TimeSpan operator*(double value) const;
  TimeSpan operator/(const TimeSpan& other) const;
  TimeSpan operator/(double value) const;
  void operator+=(const TimeSpan& other);
  void operator-=(const TimeSpan& other);
  void operator*=(const TimeSpan& other);
  void operator/=(const TimeSpan& other);

  void operator*=(double value);
  void operator/=(double value);

  bool operator==(const TimeSpan& other) const;
  bool operator!=(const TimeSpan& other) const;
  bool operator>(const TimeSpan& other) const;
  bool operator<(const TimeSpan& other) const;
  bool operator>=(const TimeSpan& other) const;
  bool operator<=(const TimeSpan& other) const;

  /**
   * @brief days
   * @return Number of days represented in the days field of this TimeSpan when printed
   */
  qint64 days() const;

  /**
   * @brief hours
   * @return Number of hours represented in the hours field of this TimeSpan when printed
   */
  qint64 hours() const;

  /**
   * @brief minutes
   * @return Number of hours represented in the minutes field of this TimeSpan when printed
   */
  qint64 minutes() const;

  /**
   * @brief seconds
   * @return Number of seconds represented in the seconds field of this TimeSpan when printed
   */
  qint64 seconds() const;

  /**
   * @brief milliseconds
   * @return Number of milliseconds represented in the miliseconds field of this TimeSpan when printed
   */
  qint64 milliseconds() const;

  /**
   * @brief microseconds
   * @return Number of microseconds represented in the microseconds field of this TimeSpan when printed
   */
  qint64 microseconds() const;

  /**
   * @brief nanoseconds
   * @return Number of nanoseconds represented in the nanoseconds field of this TimeSpan when printed
   */
  qint64 nanoseconds() const;

  /**
   * @brief totalSeconds
   * @return The total number of seconds represented in the TimeSpan
   */
  double totalSeconds() const;

  /**
   * @brief totalMinutes
   * @return The total number of minutes represented in the TimeSpan
   */
  double totalMinutes() const;

  /**
   * @brief totalHours
   * @return The total number of hours represented in the TimeSpan
   */
  double totalHours() const;

  /**
   * @brief totalDays
   * @return The total number of days represented in the TimeSpan
   */
  double totalDays() const;

  /**
   * @brief totalMilliseconds
   * @return The total number of milliseconds represented in the TimeSpan
   */
  double totalMilliseconds() const;

  /**
   * @brief totalMicroseconds
   * @return The total number of microseconds represented in the TimeSpan
   */
  double totalMicroseconds() const;

  /**
   * @brief totalNanoseconds
   * @return The total number of nanoseconds represented in the TimeSpan
   */
  double totalNanoseconds() const;

  bool isZero() const { return _nanoseconds == 0; }
  bool isNegative() const { return _nanoseconds < 0; }
  bool isPositive() const { return _nanoseconds > 0; }

  void toTimeSpec(struct timespec& timespec) const;
  QString toString(bool microseconds = false) const;
  QString toAbbreviatedFormat(bool showMilliseconds = false) const;
  QString toDumpString() const;

  TimeSpan addNanoseconds(double nanoseconds) const;
  TimeSpan addMicroseconds(double microseconds) const;
  TimeSpan addMilliseconds(double milliseconds) const;
  TimeSpan addSeconds(double seconds) const;
  TimeSpan addMinutes(double minutes) const;
  TimeSpan addHours(double hours) const;
  TimeSpan addDays(double days) const;

  static TimeSpan fromVariant(const QVariant& value) { return value.value<TimeSpan>(); }
  QVariant toVariant() const { return QVariant::fromValue<TimeSpan>(*this); }

  static TimeSpan fromNanoseconds(double nanoseconds);
  static TimeSpan fromMicroseconds(double microseconds);
  static TimeSpan fromMilliseconds(double milliseconds);
  static TimeSpan fromSeconds(double seconds);
  static TimeSpan fromMinutes(double minutes);
  static TimeSpan fromHours(double hours);
  static TimeSpan fromDays(double days);
  static TimeSpan fromString(const QString& timeString);
  static TimeSpan zero() { return TimeSpan::fromSeconds(0); }
  static TimeSpan diff(const QDateTime& now, const QDateTime& then);
  static TimeSpan diff(std::chrono::system_clock::time_point now, std::chrono::system_clock::time_point then);
  static TimeSpan absDiff(const QDateTime& t1, const QDateTime& t2);
  static TimeSpan absDiff(std::chrono::system_clock::time_point t1, std::chrono::system_clock::time_point t2);

  static TimeSpan max(const TimeSpan& t1, const TimeSpan& t2) { return t1 > t2 ? t1 : t2; }
  static TimeSpan min(const TimeSpan& t1, const TimeSpan& t2) { return t1 < t2 ? t1 : t2; }

private:
  static TimeSpan parseAbbreviatedString(const QString& timeString);
  static TimeSpan parseColonDelimitedString(const QString& timeString);
  static TimeSpan parseMicrosecondString(const QString& timeString);
  static TimeSpan parseMillisecondString(const QString& timeString);
  static QStringList getTokens(const QString &timeString);
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

Q_DECLARE_METATYPE(TimeSpan)

#endif /* TIMESPAN_H_ */

