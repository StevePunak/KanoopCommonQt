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

/**
 * @brief A signed 64-bit nanosecond-resolution duration value, similar to the .NET TimeSpan class.
 *
 * Provides arithmetic, comparison, and conversion operations across all common time units.
 * The underlying storage is a signed 64-bit nanosecond count, giving a maximum range of
 * approximately 292 years.  Use the static factory methods (fromSeconds(), fromMilliseconds(),
 * etc.) or absDiff() to construct instances conveniently.
 */
class KANOOP_EXPORT TimeSpan
{
public:
  /** @brief Default constructor — creates a zero-length time span. */
  TimeSpan() :
    _nanoseconds(0) {}

  /**
   * @brief Construct a TimeSpan from explicit time components.
   * @param days Number of days
   * @param hours Number of hours
   * @param minutes Number of minutes
   * @param seconds Number of seconds
   * @param milliseconds Number of milliseconds (default 0)
   * @param microseconds Number of microseconds (default 0)
   * @param nanoseconds Number of nanoseconds (default 0)
   */
  TimeSpan(double days, double hours, double minutes, double seconds, double milliseconds = 0, double microseconds = 0, double nanoseconds = 0);

  /**
   * @brief Copy constructor.
   * @param other TimeSpan to copy
   */
  TimeSpan(const TimeSpan& other);

  /**
   * @brief Construct a TimeSpan from a POSIX timespec value.
   * @param other timespec containing tv_sec and tv_nsec fields
   */
  TimeSpan(const timespec& other);

  /** @brief Copy-assign from another TimeSpan. */
  TimeSpan& operator=(const TimeSpan& other);
  /** @brief Assign from a POSIX timespec. */
  TimeSpan& operator=(const timespec& other);

  /** @brief Add two time spans. @return Sum */
  TimeSpan operator+(const TimeSpan& other) const;
  /** @brief Subtract a time span. @return Difference */
  TimeSpan operator-(const TimeSpan& other) const;
  /** @brief Multiply two time spans component-wise. @return Product */
  TimeSpan operator*(const TimeSpan& other) const;
  /** @brief Scale by a scalar factor. @return Scaled span */
  TimeSpan operator*(double value) const;
  /** @brief Divide two time spans component-wise. @return Quotient */
  TimeSpan operator/(const TimeSpan& other) const;
  /** @brief Divide by a scalar factor. @return Scaled span */
  TimeSpan operator/(double value) const;
  /** @brief Add-assign a TimeSpan. */
  void operator+=(const TimeSpan& other);
  /** @brief Subtract-assign a TimeSpan. */
  void operator-=(const TimeSpan& other);
  /** @brief Multiply-assign by a TimeSpan. */
  void operator*=(const TimeSpan& other);
  /** @brief Divide-assign by a TimeSpan. */
  void operator/=(const TimeSpan& other);
  /** @brief Multiply-assign by a scalar. */
  void operator*=(double value);
  /** @brief Divide-assign by a scalar. */
  void operator/=(double value);

  /** @brief Equality comparison. */
  bool operator==(const TimeSpan& other) const;
  /** @brief Inequality comparison. */
  bool operator!=(const TimeSpan& other) const;
  /** @brief Greater-than comparison. */
  bool operator>(const TimeSpan& other) const;
  /** @brief Less-than comparison. */
  bool operator<(const TimeSpan& other) const;
  /** @brief Greater-than-or-equal comparison. */
  bool operator>=(const TimeSpan& other) const;
  /** @brief Less-than-or-equal comparison. */
  bool operator<=(const TimeSpan& other) const;

  /**
   * @brief Output format for toString().
   */
  enum Format {
    Auto,           ///< Choose format automatically based on the span's magnitude
    Milliseconds,   ///< Display with millisecond precision
    MicroSeconds,   ///< Display with microsecond precision
    Abbreviated,    ///< Abbreviated format (e.g. "1h 2m 3s")
    NoMilliseconds, ///< Display without sub-second components
  };

  /**
   * Number of days represented in the days field of this super TimeSpan
   * when printed.
   *
   * @return Number of whole days represented in the days field of this TimeSpan when printed
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

  /**
   * @brief Is this timespan zero total nanoseconds
   * @return true if this timespan is zero
   */
  bool isZero() const { return _nanoseconds == 0; }
  /** @brief Return true if this time span represents a negative duration. */
  bool isNegative() const { return _nanoseconds < 0; }
  /** @brief Return true if this time span represents a positive (non-zero) duration. */
  bool isPositive() const { return _nanoseconds > 0; }
  /**
   * @brief Return the absolute value of this time span.
   * @return Non-negative copy of this TimeSpan
   */
  TimeSpan absoluteValue() const;

  /** @brief Return true if this time span was successfully constructed or parsed. */
  bool isValid() const { return _valid; }

  /**
   * @brief Populate a POSIX timespec structure from this TimeSpan.
   * @param timespec Output timespec to fill with tv_sec and tv_nsec
   */
  void toTimeSpec(struct timespec& timespec) const;

  /**
   * @brief Format this TimeSpan as a string in the specified format.
   * @param format Display format
   * @param microseconds If true, include microsecond resolution
   * @return Formatted duration string
   */
  QString toString(Format format, bool microseconds = false) const;

  /**
   * @brief Format this TimeSpan as a string using the Auto format.
   * @param microseconds If true, include microsecond resolution
   * @return Formatted duration string
   */
  QString toString(bool microseconds = false) const;

  /**
   * @brief Format this TimeSpan using the abbreviated display format (e.g. "1h 2m 3s").
   * @param showMilliseconds If true, append milliseconds to the output
   * @return Abbreviated duration string
   */
  QString toAbbreviatedFormat(bool showMilliseconds = false) const;

  /**
   * @brief Format this TimeSpan as a verbose diagnostic string showing all internal fields.
   * @return Diagnostic dump string
   */
  QString toDumpString() const;

  /**
   * @brief Return a new TimeSpan with nanoseconds added.
   * @param nanoseconds Nanoseconds to add
   * @return Adjusted TimeSpan
   */
  TimeSpan addNanoseconds(double nanoseconds) const;

  /**
   * @brief Return a new TimeSpan with microseconds added.
   * @param microseconds Microseconds to add
   * @return Adjusted TimeSpan
   */
  TimeSpan addMicroseconds(double microseconds) const;

  /**
   * @brief Return a new TimeSpan with milliseconds added.
   * @param milliseconds Milliseconds to add
   * @return Adjusted TimeSpan
   */
  TimeSpan addMilliseconds(double milliseconds) const;

  /**
   * @brief Return a new TimeSpan with seconds added.
   * @param seconds Seconds to add
   * @return Adjusted TimeSpan
   */
  TimeSpan addSeconds(double seconds) const;

  /**
   * @brief Return a new TimeSpan with minutes added.
   * @param minutes Minutes to add
   * @return Adjusted TimeSpan
   */
  TimeSpan addMinutes(double minutes) const;

  /**
   * @brief Return a new TimeSpan with hours added.
   * @param hours Hours to add
   * @return Adjusted TimeSpan
   */
  TimeSpan addHours(double hours) const;

  /**
   * @brief Return a new TimeSpan with days added.
   * @param days Days to add
   * @return Adjusted TimeSpan
   */
  TimeSpan addDays(double days) const;

  /**
   * @brief Construct a TimeSpan from a QVariant holding a TimeSpan value.
   * @param value Variant containing a TimeSpan
   * @return Extracted TimeSpan
   */
  static TimeSpan fromVariant(const QVariant& value) { return value.value<TimeSpan>(); }

  /**
   * @brief Wrap this TimeSpan in a QVariant for storage or signal passing.
   * @return QVariant containing this TimeSpan
   */
  QVariant toVariant() const { return QVariant::fromValue<TimeSpan>(*this); }

  /**
   * @brief Construct a TimeSpan from a nanosecond count.
   * @param nanoseconds Duration in nanoseconds
   * @return TimeSpan
   */
  static TimeSpan fromNanoseconds(double nanoseconds);

  /**
   * @brief Construct a TimeSpan from a microsecond count.
   * @param microseconds Duration in microseconds
   * @return TimeSpan
   */
  static TimeSpan fromMicroseconds(double microseconds);

  /**
   * @brief Construct a TimeSpan from a millisecond count.
   * @param milliseconds Duration in milliseconds
   * @return TimeSpan
   */
  static TimeSpan fromMilliseconds(double milliseconds);

  /**
   * @brief Construct a TimeSpan from a second count.
   * @param seconds Duration in seconds
   * @return TimeSpan
   */
  static TimeSpan fromSeconds(double seconds);

  /**
   * @brief Construct a TimeSpan from a minute count.
   * @param minutes Duration in minutes
   * @return TimeSpan
   */
  static TimeSpan fromMinutes(double minutes);

  /**
   * @brief Construct a TimeSpan from an hour count.
   * @param hours Duration in hours
   * @return TimeSpan
   */
  static TimeSpan fromHours(double hours);

  /**
   * @brief Construct a TimeSpan from a day count.
   * @param days Duration in days
   * @return TimeSpan
   */
  static TimeSpan fromDays(double days);

  /**
   * @brief Parse a TimeSpan from a formatted duration string.
   * @param timeString String representation of a time span
   * @param parsed Optional output flag set to true if parsing succeeded
   * @return Parsed TimeSpan, or an invalid sentinel on failure
   */
  static TimeSpan fromString(const QString& timeString, bool* parsed = nullptr);

  /** @brief Return a zero-length TimeSpan. */
  static TimeSpan zero() { return TimeSpan::fromSeconds(0); }

  /** @brief Return an invalid (sentinel) TimeSpan whose isValid() returns false. */
  static TimeSpan invalid();

  /**
   * @brief Compute the signed difference between two QDateTime values (now − then).
   * @param now Minuend QDateTime
   * @param then Subtrahend QDateTime
   * @return Signed difference TimeSpan
   */
  static TimeSpan diff(const QDateTime& now, const QDateTime& then);

  /**
   * @brief Compute the signed difference between two chrono time points (now − then).
   * @param now Minuend time point
   * @param then Subtrahend time point
   * @return Signed difference TimeSpan
   */
  static TimeSpan diff(std::chrono::system_clock::time_point now, std::chrono::system_clock::time_point then);

  /**
   * @brief Compute the absolute (non-negative) difference between two QDateTime values.
   * @param t1 First QDateTime
   * @param t2 Second QDateTime
   * @return Non-negative difference TimeSpan
   */
  static TimeSpan absDiff(const QDateTime& t1, const QDateTime& t2);

  /**
   * @brief Compute the absolute difference between two chrono time points.
   * @param t1 First time point
   * @param t2 Second time point
   * @return Non-negative difference TimeSpan
   */
  static TimeSpan absDiff(std::chrono::system_clock::time_point t1, std::chrono::system_clock::time_point t2);

  /**
   * @brief Return the larger of two time spans.
   * @param t1 First TimeSpan
   * @param t2 Second TimeSpan
   * @return The greater of t1 and t2
   */
  static TimeSpan max(const TimeSpan& t1, const TimeSpan& t2) { return t1 > t2 ? t1 : t2; }

  /**
   * @brief Return the smaller of two time spans.
   * @param t1 First TimeSpan
   * @param t2 Second TimeSpan
   * @return The lesser of t1 and t2
   */
  static TimeSpan min(const TimeSpan& t1, const TimeSpan& t2) { return t1 < t2 ? t1 : t2; }

private:
  static TimeSpan parseAbbreviatedString(const QString& timeString, bool* parsed = nullptr);
  static TimeSpan parseColonDelimitedString(const QString& timeString, bool* parsed = nullptr);
  static TimeSpan parseMicrosecondString(const QString& timeString, bool* parsed = nullptr);
  static TimeSpan parseMillisecondString(const QString& timeString, bool* parsed = nullptr);
  static QStringList getTokens(const QString &timeString);
  static int parseIntToToken(QString &remaining, const QString &until);

  qint64  _nanoseconds;
  bool    _valid = true;

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
