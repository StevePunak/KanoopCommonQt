#ifndef DATETIMEUTIL_H
#define DATETIMEUTIL_H
#include <QDateTime>
#include <QString>
#include <QVariant>

class DateTimeUtil
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
    static QString toSquashedString(QDateTime date) { return date.toString("yyyyMMddhhmmsszzz"); }
};

#endif // DATETIMEUTIL_H
