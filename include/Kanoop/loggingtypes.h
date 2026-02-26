/******************************************************************************************
**
** loggingtypes.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Tue Oct 3 09:44:40 2023
**
******************************************************************************************/
#ifndef LOGGINGTYPES_H
#define LOGGINGTYPES_H
#include <Kanoop/kanoopcommon.h>

/**
 * @brief Logging subsystem namespace.
 */
namespace Log {

/**
 * @brief Severity levels for log messages, ordered from most to least critical.
 *
 * Mirrors the syslog severity levels. The active logger level acts as a threshold:
 * messages with a value greater than the threshold are discarded.
 */
enum LogLevel
{
    Emergency       = 0,    ///< System is unusable
    Alert           = 1,    ///< Action must be taken immediately
    Critical        = 2,    ///< Critical conditions
    Error           = 3,    ///< Error conditions
    Warning         = 4,    ///< Warning conditions
    Notice          = 5,    ///< Normal but significant condition
    Info            = 6,    ///< Informational messages
    Debug           = 7,    ///< Debug-level messages
};

/**
 * @brief Bitmask flags controlling where log output is sent.
 */
enum OutputFlags
{
    None                        = 0x00000000,   ///< No output flags set
    LineNumbers                 = 0x00000001,   ///< Include source line numbers in output
    Timestamp                   = 0x00000002,   ///< Include timestamps in output
    Level                       = 0x00000004,   ///< Include severity level in output
    Console                     = 0x00001000,   ///< Write to stdout/stderr
    File                        = 0x00002000,   ///< Write to a log file
    QDebug                      = 0x00004000,   ///< Write via Qt's qDebug() mechanism
    Syslog                      = 0x00008000,   ///< Write to the system syslog

    Standard = LineNumbers | Timestamp | Level | Console   ///< Default output configuration
};

/**
 * @brief Bidirectional map between LogLevel values and their string names.
 */
class LogLevelToStringMap : public KANOOP::EnumToStringMap<LogLevel>
{
public:
    /** @brief Populate the map with all standard LogLevel name pairs. */
    LogLevelToStringMap()
    {
        insert(Emergency,   "Emergency");
        insert(Alert,       "Alert");
        insert(Critical,    "Critical");
        insert(Error,       "Error");
        insert(Warning,     "Warning");
        insert(Notice,      "Notice");
        insert(Info,        "Info");
        insert(Debug,       "Debug");
    }
};

/**
 * @brief Look up a LogLevel by its name string.
 * @param levelName Name string (case-insensitive)
 * @return Matching LogLevel, or Debug as default
 */
KANOOP_EXPORT LogLevel getLogLevel(const QString& levelName);

/**
 * @brief Return a list of all defined LogLevel values.
 * @return List of LogLevel enum values
 */
KANOOP_EXPORT QList<LogLevel> getLogLevels();

/**
 * @brief Return the name string for a LogLevel value.
 * @param level LogLevel to convert
 * @return Name string, e.g. "Debug"
 */
KANOOP_EXPORT QString getLogLevelString(LogLevel level);

} // namespace Log

#endif // LOGGINGTYPES_H
