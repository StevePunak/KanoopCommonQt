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

namespace Log {

enum LogLevel
{
    Emergency       = 0,    // 0
    Alert           = 1,    // 1
    Critical        = 2,    // 2
    Error           = 3,    // 3
    Warning         = 4,    // 4
    Notice          = 5,    // 5
    Info            = 6,    // 6
    Debug           = 7,    // 7
};

enum OutputFlags
{
    None                        = 0x00000000,
    LineNumbers                 = 0x00000001,
    Timestamp                   = 0x00000002,
    Level                       = 0x00000004,
    Console                     = 0x00001000,
    File                        = 0x00002000,
    QDebug                      = 0x00004000,
    Syslog                      = 0x00008000,

    Standard = LineNumbers | Timestamp | Level | Console
};

class LogLevelToStringMap : public KANOOP::EnumToStringMap<LogLevel>
{
public:
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

KANOOP_EXPORT LogLevel getLogLevel(const QString& levelName);
KANOOP_EXPORT QList<LogLevel> getLogLevels();
KANOOP_EXPORT QString getLogLevelString(LogLevel level);

} // namespace Log

#endif // LOGGINGTYPES_H
