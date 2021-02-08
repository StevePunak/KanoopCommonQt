#include "datetimeutil.h"


QDateTime DateTimeUtil::fromString(const QString &date)
{
    QDateTime result;
    if( (result = fromISOString(date)).isValid() ||
        (result = fromStandardString(date)).isValid() ||
        (result = QDateTime::fromString(date)).isValid())
    {
    }
    return result;
}
