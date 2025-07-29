#ifndef LOGENTRY_H
#define LOGENTRY_H
#include <QDateTime>
#include <QObject>
#include <Kanoop/logcategory.h>
#include <Kanoop/log.h>

namespace Log {

class KANOOP_EXPORT LogEntry
{
public:
    LogEntry() {}
    LogEntry(LogLevel level, const LogCategory& category, const QDateTime& timestamp, const QString& formattedText, const QString& unformattedText) :
        _level(level), _category(category), _timestamp(timestamp), _formattedText(formattedText), _unformattedText(unformattedText) {}

    LogLevel level() const { return _level; }
    Log::LogCategory category() const { return _category; }
    QDateTime timestamp() const { return _timestamp; }
    QString formattedText() const { return _formattedText; }
    QString unformattedText() const { return _unformattedText; }

private:
    LogLevel _level = Debug;
    Log::LogCategory _category;
    QDateTime _timestamp;
    QString _formattedText;
    QString _unformattedText;
};

} // namespace Log


#endif // LOGENTRY_H
