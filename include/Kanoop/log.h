/******************************************************************************************
**
** log.h
**
** Consistent formatted logging with syslog integration.
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Wed Oct 4 13:25:38 2023
**
******************************************************************************************/
#ifndef LOG_H
#define LOG_H
#include <QFile>
#include <QMap>
#include <QMutex>
#include <QString>
#include <QTextStream>
#include "loggingtypes.h"
#include <Kanoop/kanoopcommon.h>

/** @brief Logging macro helpers — expand to file, line, and level arguments. */
#define LVL_EMERGENCY       __FILE__,__LINE__,Log::LogLevel::Emergency
#define LVL_ALERT           __FILE__,__LINE__,Log::LogLevel::Alert
#define LVL_CRITICAL        __FILE__,__LINE__,Log::LogLevel::Critical
#define LVL_ERROR           __FILE__,__LINE__,Log::LogLevel::Error
#define LVL_WARNING         __FILE__,__LINE__,Log::LogLevel::Warning
#define LVL_NOTICE          __FILE__,__LINE__,Log::LogLevel::Notice
#define LVL_INFO            __FILE__,__LINE__,Log::LogLevel::Info
#define LVL_DEBUG           __FILE__,__LINE__,Log::LogLevel::Debug


class LogConsumer;

/**
 * @brief Logging subsystem providing categorized, level-filtered output.
 */
namespace Log
{

class LogCategoryPrivate;

class LogCategory;

/**
 * @brief Core logger class supporting console, file, syslog, and consumer outputs.
 *
 * Logger is typically accessed through the module-level free functions (Log::logText, etc.)
 * which delegate to a process-wide singleton returned by Log::systemLog().
 */
class KANOOP_EXPORT Logger
{
public:
    /** @brief Default constructor — initializes an unopened logger at Debug level. */
    Logger();

    /** @brief Open the log file or syslog connection as configured. */
    void openLog();

    /**
     * @brief Rotate the log to a new file path.
     * @param newFileName Path to the new log file
     */
    void rotateLog(const QString &newFileName);

    /**
     * @brief Write a text message to the log.
     * @param file Source filename (use __FILE__)
     * @param lineNumber Source line number (use __LINE__)
     * @param level Severity level of this message
     * @param text Message text to log
     */
    void logText(const char* file, int lineNumber, LogLevel level, const QString& text);

    /**
     * @brief Write a categorized text message to the log.
     * @param file Source filename (use __FILE__)
     * @param lineNumber Source line number (use __LINE__)
     * @param level Severity level of this message
     * @param category Log category for filtering
     * @param text Message text to log
     */
    void logText(const char* file, int lineNumber, LogLevel level, const LogCategory& category, const QString& text);

    /**
     * @brief Write a hex dump of a byte array to the log.
     * @param file Source filename (use __FILE__)
     * @param lineNumber Source line number (use __LINE__)
     * @param level Severity level of this message
     * @param data Bytes to dump
     * @param tag Optional label prepended to the dump
     */
    void logHex(const char* file, int lineNumber, LogLevel level, const QByteArray& data, const QString& tag = QString());

    /**
     * @brief Write a categorized hex dump to the log.
     * @param file Source filename (use __FILE__)
     * @param lineNumber Source line number (use __LINE__)
     * @param level Severity level of this message
     * @param category Log category for filtering
     * @param data Bytes to dump
     * @param tag Optional label prepended to the dump
     */
    void logHex(const char* file, int lineNumber, LogLevel level, const LogCategory& category, const QByteArray& data, const QString& tag = QString());

    /**
     * @brief Return the current minimum log level.
     * @return Active LogLevel threshold
     */
    LogLevel level() const { return _level; }

    /**
     * @brief Set the minimum log level; messages below this level are discarded.
     * @param value New LogLevel threshold
     */
    void setLevel(LogLevel value) { _level = value; }

    /**
     * @brief Return the active output flags.
     * @return Bitmask of OutputFlags
     */
    OutputFlags flags() const { return _flags; }

    /**
     * @brief Set the output flags, replacing any existing flags.
     * @param flags New OutputFlags bitmask
     */
    void setFlags(OutputFlags flags) { _flags = flags; }

    /**
     * @brief Enable additional output flags without clearing existing ones.
     * @param flags Flags to enable
     */
    void enableOutputFlags(OutputFlags flags);

    /**
     * @brief Disable specific output flags without affecting others.
     * @param flags Flags to disable
     */
    void disableOutputFlags(OutputFlags flags);

    /**
     * @brief Return the log file path.
     * @return Path to the output log file
     */
    QString filename() const { return _filename; }

    /**
     * @brief Set the log file path and reopen the file.
     * @param filename Path to the output log file
     */
    void setFilename(const QString& filename);

    /**
     * @brief Return the syslog identity string.
     * @return Identity string used in syslog output
     */
    QString identity() const { return _identity; }

    /**
     * @brief Set the syslog identity string.
     * @param value Identity string used in syslog output
     */
    void setIdentity(const QString& value);

    /**
     * @brief Register a new log category by name.
     * @param name Category name string
     * @return Registered LogCategory object
     */
    LogCategory registerCategory(const QString& name);

    /**
     * @brief Register an existing LogCategory object.
     * @param category Category to register
     * @return Registered LogCategory object
     */
    LogCategory registerCategory(const LogCategory& category);

    /**
     * @brief Return all registered log categories.
     * @return List of LogCategory objects
     */
    QList<LogCategory> categories() const;

    /**
     * @brief Override the log level for a specific category.
     * @param name Category name
     * @param level Level to apply to this category
     */
    void setCategoryLevel(const QString& name, LogLevel level);

    /**
     * @brief Add an external log consumer to receive log entries.
     * @param consumer Consumer to add
     */
    void addConsumer(LogConsumer* consumer);

    /**
     * @brief Remove a previously added log consumer.
     * @param consumer Consumer to remove
     */
    void removeConsumer(LogConsumer* consumer);

    /**
     * @brief Test whether the log is open and ready to write.
     * @return true if the log is open
     */
    bool isLogOpen() const { return _logOpen; }

private:
    class CategoryIndex : public QMap<QString, LogCategoryPrivate*>
    {
    };

    /** @brief Open the log file on disk. */
    void openFile();
    /** @brief Close the log file. */
    void closeFile();

    /** @brief Open the syslog connection. */
    void openSyslog();
    /** @brief Close the syslog connection. */
    void closeSyslog();

    /** @brief Route a formatted log message to all enabled output destinations. */
    void outputToDestinations(LogLevel level, const LogCategory& category, const QDateTime& timestamp, const QString& formattedText, const QString& unformattedText);

    /** @brief Convert raw bytes to a formatted hex string. */
    static QString bufferToHex(const QByteArray& buffer);

    LogLevel _level;
    OutputFlags _flags;
    QString _filename;
    char _identity[512];

    bool _logOpen;

    QTextStream _stdout;
    QTextStream _stderr;

    QList<LogConsumer*> _surplusConsumers;

    QFile _file;
    QMutex _writeLock;

    CategoryIndex _categories;

    static const QList<QString> _LevelStrings;
};

/**
 * @brief Write a text message via the system logger.
 * @param file Source filename
 * @param lineNumber Source line number
 * @param level Severity level
 * @param text Message text
 */
KANOOP_EXPORT void logText(const char* file, int lineNumber, LogLevel level, const QString& text);

/**
 * @brief Write a categorized text message via the system logger.
 * @param file Source filename
 * @param lineNumber Source line number
 * @param level Severity level
 * @param category Log category
 * @param text Message text
 */
KANOOP_EXPORT void logText(const char* file, int lineNumber, LogLevel level, const LogCategory& category, const QString& text);

/**
 * @brief Write a categorized hex dump via the system logger.
 * @param file Source filename
 * @param lineNumber Source line number
 * @param level Severity level
 * @param category Log category
 * @param data Bytes to dump
 * @param tag Optional label
 */
KANOOP_EXPORT void logHex(const char* file, int lineNumber, LogLevel level, const LogCategory& category, const QByteArray& data, const QString& tag = QString());

/**
 * @brief Write a hex dump via the system logger.
 * @param file Source filename
 * @param lineNumber Source line number
 * @param level Severity level
 * @param data Bytes to dump
 * @param tag Optional label
 */
KANOOP_EXPORT void logHex(const char* file, int lineNumber, LogLevel level, const QByteArray& data, const QString& tag = QString());

/**
 * @brief Return the process-wide Logger singleton.
 * @return Pointer to the global Logger instance
 */
KANOOP_EXPORT Logger* systemLog();

/**
 * @brief Return the current minimum log level of the system logger.
 * @return Active LogLevel threshold
 */
KANOOP_EXPORT LogLevel level();

/**
 * @brief Set the minimum log level of the system logger.
 * @param value New LogLevel threshold
 */
KANOOP_EXPORT void setLevel(LogLevel value);

/**
 * @brief Return the output flags of the system logger.
 * @return Active OutputFlags bitmask
 */
KANOOP_EXPORT OutputFlags flags();

/**
 * @brief Set the output flags of the system logger.
 * @param flags New OutputFlags bitmask
 */
KANOOP_EXPORT void setFlags(OutputFlags flags);

/**
 * @brief Return the log file path of the system logger.
 * @return Log file path string
 */
KANOOP_EXPORT QString filename();

/**
 * @brief Set the log file path of the system logger.
 * @param filename Path to the log file
 */
KANOOP_EXPORT void setFilename(const QString& filename);

/**
 * @brief Return the syslog identity of the system logger.
 * @return Syslog identity string
 */
KANOOP_EXPORT QString identity();

/**
 * @brief Set the syslog identity of the system logger.
 * @param value Identity string
 */
KANOOP_EXPORT void setIdentity(const QString& value);

/**
 * @brief Enable additional output flags on the system logger.
 * @param flags Flags to enable
 */
KANOOP_EXPORT void enableOutputFlags(OutputFlags flags);

/**
 * @brief Disable specific output flags on the system logger.
 * @param flags Flags to disable
 */
KANOOP_EXPORT void disableOutputFlags(OutputFlags flags);

/**
 * @brief Register a log category by name with the system logger.
 * @param name Category name
 * @return Registered LogCategory
 */
KANOOP_EXPORT LogCategory registerCategory(const QString& name);

/**
 * @brief Register a log category by name and level with the system logger.
 * @param name Category name
 * @param level Level override for this category
 * @return Registered LogCategory
 */
KANOOP_EXPORT LogCategory registerCategory(const QString &name, LogLevel level);

/**
 * @brief Return all categories registered with the system logger.
 * @return List of LogCategory objects
 */
KANOOP_EXPORT QList<LogCategory> categories();

/**
 * @brief Override the log level for a named category on the system logger.
 * @param name Category name
 * @param level Level to apply
 */
KANOOP_EXPORT void setCategoryLevel(const QString& name, LogLevel level);

/**
 * @brief Add an external log consumer to the system logger.
 * @param consumer Consumer to add
 */
KANOOP_EXPORT void addConsumer(LogConsumer* consumer);

/**
 * @brief Remove an external log consumer from the system logger.
 * @param consumer Consumer to remove
 */
KANOOP_EXPORT void removeConsumer(LogConsumer* consumer);

/**
 * @brief Parse a log level name string into a LogLevel enum value.
 * @param value String name (e.g. "debug", "error")
 * @param parsed Optional output set to true if parsing succeeded
 * @return Parsed LogLevel, or Debug as default on failure
 */
KANOOP_EXPORT LogLevel parseLevel(const QString& value, bool* parsed = nullptr);

/**
 * @brief Read the start timestamp from an existing log file.
 * @param filename Path to the log file
 * @return QDateTime of the first log entry, or invalid QDateTime on error
 */
KANOOP_EXPORT QDateTime getLogStartTime(const QString& filename);

/**
 * @brief Read the end timestamp from an existing log file.
 * @param filename Path to the log file
 * @return QDateTime of the last log entry, or invalid QDateTime on error
 */
KANOOP_EXPORT QDateTime getLogEndTime(const QString& filename);


} // namespace log

#endif // LOG_H
