#ifndef DATETIMEUTIL_H
#define DATETIMEUTIL_H
#include <QDateTime>
#include <QString>
#include <QVariant>
#include "kanoopcommon.h"

class KANOOP_EXPORT DateTimeUtil
{
public:
    static QDateTime fromISOString(const QString& date)
    {
        QDateTime result = QDateTime::fromString(date, Qt::DateFormat::ISODateWithMs);
        result.setTimeSpec(Qt::UTC);
        return result;
    }
    static QDateTime fromStandardString(const QString& date)
    {
        QDateTime result = QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss.zzz");
        result.setTimeSpec(Qt::UTC);
        return result;
    }
    static QDateTime fromSquashedString(const QString& date);
    static QDateTime fromString(const QString& date);
    static QDateTime fromVariant(const QVariant& date);

    static QString toStandardString(const QDateTime& date) { return date.toString("yyyy-MM-dd hh:mm:ss.zzz"); }
    static QString toISOString(const QDateTime& date) { return date.toString("yyyy-MM-ddThh:mm:ss.zzzZ"); }
    static QString toISOString() { return QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddThh:mm:ss.zzzZ"); }
    static QString toStandardString() { return QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss.zzz"); }
    static QString toSquashedString() { return QDateTime::currentDateTimeUtc().toString("yyyyMMddhhmmsszzz"); }
    static QString toSquashedString(const QDateTime& date) { return date.toString("yyyyMMddhhmmsszzz"); }
    static QString toStandardTimeString(const QDateTime& date) { return date.time().toString("HH:mm:ss.zzz"); }

private:
    /**
     * @brief fromAlternate1String
     * Parse this format: 2021-12-17 Friday 22:22:51.569
     * @return
     */
    static QDateTime fromAlternate1String(const QString& date)
    {
        QDateTime result = QDateTime::fromString(date, "yyyy-MM-dd dddd hh:mm:ss.zzz");
        result.setTimeSpec(Qt::UTC);
        return result;
    }
    static QDateTime fromGitRepresentation(const QString& date)
    {
        QDateTime dateTime = QDateTime::fromString(date, "ddd MMM d hh:mm:ss yyyy tt").toUTC();
        return dateTime;
    }
};

#endif // DATETIMEUTIL_H
