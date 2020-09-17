#ifndef DATETIMEUTIL_H
#define DATETIMEUTIL_H
#include <QDateTime>
#include <QString>

class DateTimeUtil
{
public:
    static QString toStandardString(const QDateTime& date) { return date.toString("yyyy-MM-ddThh:mm:ss.zzzZ"); }
    static QString toStandardString() { return QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddThh:mm:ss.zzzZ"); }
    static QDateTime fromStandardString(const QString& date) { return QDateTime::fromString(date, Qt::DateFormat::ISODateWithMs); }
};

#endif // DATETIMEUTIL_H
