#include "Kanoop/datetimeutil.h"


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
