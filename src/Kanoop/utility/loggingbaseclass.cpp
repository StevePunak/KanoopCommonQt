#include "utility/loggingbaseclass.h"
#include <Kanoop/log.h>

LoggingBaseClass::LoggingBaseClass(const Log::LogCategory &category) :
    _baseCategory(category)
{
    commonInit();
}

LoggingBaseClass::LoggingBaseClass(const QString &category) :
    _baseCategory(Log::LogCategory(category))
{
    commonInit();
}

void LoggingBaseClass::commonInit()
{
    if(_baseCategory.isValid()) {
        _baseCategory = Log::registerCategory(_baseCategory.name());
        _level1 = Log::registerCategory(_baseCategory.append("lvl1").name());
        _level2 = Log::registerCategory(_baseCategory.append("lvl2").name());
        _level3 = Log::registerCategory(_baseCategory.append("lvl3").name());
    }
}

void LoggingBaseClass::logText(const char *file, int lineNumber, Log::LogLevel level, const QString &text) const
{
    logText(file, lineNumber, level, LVL0(), text);
}

void LoggingBaseClass::logText(const char *file, int lineNumber, Log::LogLevel level, const Log::LogCategory &category, const QString &text) const
{
    Log::logText(file, lineNumber, level, category, text);
}

void LoggingBaseClass::logHex(const char *file, int lineNumber, Log::LogLevel level, const QByteArray &data, const QString &tag) const
{
    Log::logHex(file, lineNumber, level, LVL0(), data, tag);
}

void LoggingBaseClass::logHex(const char *file, int lineNumber, Log::LogLevel level, const Log::LogCategory &category, const QByteArray &data, const QString &tag) const
{
    Log::logHex(file, lineNumber, level, category, data, tag);
}

void LoggingBaseClass::addLogConsumer(LogConsumer* consumer)
{
    Log::addConsumer(consumer);
}

void LoggingBaseClass::removeLogConsumer(LogConsumer* consumer)
{
    Log::removeConsumer(consumer);
}

