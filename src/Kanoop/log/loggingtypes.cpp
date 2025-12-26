#include <Kanoop/loggingtypes.h>

namespace Log {

const LogLevelToStringMap _LogLevelToStringMap;


LogLevel getLogLevel(const QString& levelName)
{
    return _LogLevelToStringMap.getType(levelName);
}

QList<LogLevel> getLogLevels()
{
    return _LogLevelToStringMap.getTypes();
}

QString getLogLevelString(LogLevel level)
{
    return _LogLevelToStringMap.getString(level);
}

}   // namespace Log
