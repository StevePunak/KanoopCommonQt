/**
 *  DateTimeUtil
 *
 *  Some static helper methods for converting date/times to and from strings.
 *
 *  In general, on of two formats are preferred:
 *    1. ISO-8601 ZULU      e.g. 2021-09-17T05:30:00.123Z
 *    2. MySQL              e.g. 2021-09-17 05:30:00.123
 *
 *  Stephen Punak, September 17 2020
 */
#ifndef DATETIMEUTIL_H
#define DATETIMEUTIL_H
#include <QDateTime>
#include <QString>
#include <QTimeZone>
#include <QVariant>
#include "kanoopcommon.h"

/**
 * @brief Static helper methods for parsing and formatting QDateTime values.
 *
 * Supports ISO-8601 Zulu (e.g. 2021-09-17T05:30:00.123Z) and MySQL-style
 * (e.g. 2021-09-17 05:30:00.123) formats as well as a compact "squashed" form.
 */
class KANOOP_EXPORT DateTimeUtil
{
public:
    /**
     * @brief Parse an ISO-8601 string into a UTC QDateTime.
     * @param date ISO-8601 date/time string with milliseconds
     * @return Parsed UTC QDateTime, or invalid QDateTime on error
     */
    static QDateTime fromISOString(const QString& date)
    {
        QDateTime result = QDateTime::fromString(date, Qt::DateFormat::ISODateWithMs);
        result.setTimeZone(QTimeZone::utc());
        return result;
    }

    /**
     * @brief Parse a MySQL-style "yyyy-MM-dd hh:mm:ss.zzz" string into a QDateTime.
     * @param date MySQL format date/time string
     * @return Parsed QDateTime, or invalid QDateTime on error
     */
    static QDateTime fromStandardString(const QString& date);

    /**
     * @brief Parse a compact "yyyyMMddhhmmsszzz" string into a QDateTime.
     * @param date Squashed format date/time string
     * @return Parsed QDateTime, or invalid QDateTime on error
     */
    static QDateTime fromSquashedString(const QString& date);

    /**
     * @brief Parse a date/time string, trying multiple known formats.
     * @param date Date/time string in any supported format
     * @return Parsed QDateTime, or invalid QDateTime if no format matches
     */
    static QDateTime fromString(const QString& date);

    /**
     * @brief Parse a QVariant containing a date/time string or QDateTime.
     * @param date Variant holding a date/time value
     * @return Parsed QDateTime, or invalid QDateTime on error
     */
    static QDateTime fromVariant(const QVariant& date);

    /**
     * @brief Format a QDateTime as a MySQL-style string.
     * @param date Date/time to format
     * @param milliseconds Whether to include milliseconds (default true)
     * @return Formatted string, e.g. "2021-09-17 05:30:00.123"
     */
    static QString toStandardString(const QDateTime& date, bool milliseconds = true)
    {
        return milliseconds ? date.toString("yyyy-MM-dd hh:mm:ss.zzz") : date.toString("yyyy-MM-dd hh:mm:ss");
    }

    /**
     * @brief Format a QDateTime as an ISO-8601 Zulu string.
     * @param date Date/time to format
     * @return Formatted string, e.g. "2021-09-17T05:30:00.123Z"
     */
    static QString toISOString(const QDateTime& date) { return date.toString("yyyy-MM-ddThh:mm:ss.zzzZ"); }

    /**
     * @brief Format a QDateTime as a compact squashed string.
     * @param date Date/time to format
     * @return Formatted string, e.g. "20210917053000123"
     */
    static QString toSquashedString(const QDateTime& date) { return date.toString("yyyyMMddhhmmsszzz"); }

    /**
     * @brief Format the time portion of a QDateTime as "HH:mm:ss.zzz".
     * @param date Date/time whose time portion to format
     * @return Formatted time string
     */
    static QString toStandardTimeString(const QDateTime& date) { return date.time().toString("HH:mm:ss.zzz"); }

    /**
     * @brief Return the current UTC time as an ISO-8601 Zulu string.
     * @return Current UTC timestamp string
     */
    static QString currentToISOString() { return QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddThh:mm:ss.zzzZ"); }

    /**
     * @brief Return the current UTC time as a MySQL-style string.
     * @param milliseconds Whether to include milliseconds (default true)
     * @return Current UTC timestamp string
     */
    static QString currentToStandardString(bool milliseconds = true)
    {
        return milliseconds ? QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss.zzz") : QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss");
    }

    /**
     * @brief Return the current UTC time as a compact squashed string.
     * @return Current UTC timestamp in squashed format
     */
    static QString currentToSquashedString() { return QDateTime::currentDateTimeUtc().toString("yyyyMMddhhmmsszzz"); }

private:
    /**
     * @brief fromAlternate1String
     * Parse this format: 2021-12-17 Friday 22:22:51.569
     * @return
     */
    static QDateTime fromAlternate1String(const QString& date)
    {
        QDateTime result = QDateTime::fromString(date, "yyyy-MM-dd dddd hh:mm:ss.zzz");
        result.setTimeZone(QTimeZone::utc());
        return result;
    }
    static QDateTime fromGitRepresentation(const QString& date)
    {
        QDateTime dateTime = QDateTime::fromString(date, "ddd MMM d hh:mm:ss yyyy tt").toUTC();
        return dateTime;
    }
};

#endif // DATETIMEUTIL_H
