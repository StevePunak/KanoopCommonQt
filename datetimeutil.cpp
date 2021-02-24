#include "datetimeutil.h"


QDateTime DateTimeUtil::fromString(const QString &date)
{
    QDateTime result;
    if( (result = fromISOString(date)).isValid() ||
        (result = fromStandardString(date)).isValid() ||
        (result = fromSquashedString(date)).isValid() ||
        (result = QDateTime::fromString(date)).isValid())
    {
    }
    return result;
}

QDateTime DateTimeUtil::fromVariant(const QVariant &date)
{
    QDateTime result = date.toDateTime();
    result.setOffsetFromUtc(0);
    return result;
}
