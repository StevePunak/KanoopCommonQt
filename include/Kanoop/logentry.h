/**
 * @brief A single log entry carrying level, category, timestamp, and message text.
 */
#ifndef LOGENTRY_H
#define LOGENTRY_H
#include <QDateTime>
#include <QObject>
#include <Kanoop/logcategory.h>
#include <Kanoop/log.h>

namespace Log {

/**
 * @brief Immutable record of a single log event.
 *
 * LogEntry objects are created by Logger and delivered to LogConsumer instances.
 * They capture the severity level, category, timestamp, and both the raw and
 * pre-formatted message text.
 */
class KANOOP_EXPORT LogEntry
{
public:
    /** @brief Default constructor — creates an empty Debug-level entry. */
    LogEntry() {}

    /**
     * @brief Construct a LogEntry with all fields.
     * @param level Severity level of this entry
     * @param category Log category associated with this entry
     * @param timestamp When this entry was generated
     * @param formattedText Pre-formatted message text (includes level, timestamp, etc.)
     * @param unformattedText Raw message text without formatting
     */
    LogEntry(LogLevel level, const LogCategory& category, const QDateTime& timestamp, const QString& formattedText, const QString& unformattedText) :
        _level(level), _category(category), _timestamp(timestamp), _formattedText(formattedText), _unformattedText(unformattedText) {}

    /**
     * @brief Return the severity level.
     * @return LogLevel of this entry
     */
    LogLevel level() const { return _level; }

    /**
     * @brief Return the log category.
     * @return LogCategory associated with this entry
     */
    Log::LogCategory category() const { return _category; }

    /**
     * @brief Return the timestamp when this entry was created.
     * @return QDateTime of the log event
     */
    QDateTime timestamp() const { return _timestamp; }

    /**
     * @brief Return the pre-formatted message text.
     * @return Message string including level, timestamp, and line number decorations
     */
    QString formattedText() const { return _formattedText; }

    /**
     * @brief Return the raw, unformatted message text.
     * @return Plain message string without any logging decorations
     */
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
