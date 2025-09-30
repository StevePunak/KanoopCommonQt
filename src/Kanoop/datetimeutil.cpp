#include "Kanoop/datetimeutil.h"


QDateTime DateTimeUtil::fromStandardString(const QString& date)
{
    static const QStringList formats = {
        "yyyy-MM-dd hh:mm:ss.zzz",
        "yyyy-MM-dd hh:mm:ss",
        "yyyy-MM-dd",
        "M-d-yyyy",
        "M/d/yyyy",
        "M-d-yyyy hh:mm:ss",
        "M/d/yyyy hh:mm:ss",
        "yyyy/MM/dd hh:mm:ss.zzz",
        "yyyy/MM/dd hh:mm:ss",
        "yyyy/MM/dd",
    };
    QDateTime result;
    for(const QString& format : formats) {
        if((result = QDateTime::fromString(date, format)).isValid()) {
            break;
        }
    }
    result.setTimeZone(QTimeZone::utc());
    return result;
}

QDateTime DateTimeUtil::fromSquashedString(const QString& date)
{
    QDateTime result;
    if( (result = QDateTime::fromString(date, "yyyyMMddhhmmsszzz")).isValid() ||
        (result = QDateTime::fromString(date, "yyyyMMddhhmmss")).isValid() ||
        (result = QDateTime::fromString(date, "yyyyMMddhhmm")).isValid() ||
        (result = QDateTime::fromString(date, "yyyyMMddhh")).isValid() ||
        (result = QDateTime::fromString(date, "yyyyMMdd")).isValid())
        result.setTimeZone(QTimeZone::utc());
    return result;
}

QDateTime DateTimeUtil::fromString(const QString &date)
{
    QDateTime result;
    if( (result = fromISOString(date)).isValid() ||
        (result = fromStandardString(date)).isValid() ||
        (result = fromSquashedString(date)).isValid() ||
        (result = QDateTime::fromString(date, Qt::ISODateWithMs)).isValid() ||
        (result = QDateTime::fromString(date)).isValid() ||
        (result = fromGitRepresentation(date)).isValid() ||
        (result = fromAlternate1String(date)).isValid())
    {
    }
    return result;
}

QDateTime DateTimeUtil::fromVariant(const QVariant &date)
{
    QDateTime result = date.toDateTime();
    result.setTimeZone(QTimeZone::utc());
    return result;
}
