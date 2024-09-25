#ifndef LOGGINGBASECLASS_H
#define LOGGINGBASECLASS_H

#include <Kanoop/logcategory.h>
#include <Kanoop/log.h>
#include <Kanoop/kanoopcommon.h>

#define UNIMPLEMENTED  logText(LVL_WARNING, QString("---------------- %1 UNIMPLEMENTED  ----------------").arg(__FUNCTION__));

class KANOOP_EXPORT LoggingBaseClass
{
public:
    LoggingBaseClass(const Log::LogCategory& category = Log::LogCategory());
    LoggingBaseClass(const QString& category);

protected:
    Log::LogCategory LVL0() const { return _baseCategory; }
    Log::LogCategory LVL1() const { return _level1; }
    Log::LogCategory LVL2() const { return _level2; }
    Log::LogCategory LVL3() const { return _level3; }

    void logText(const char* file, int lineNumber, Log::LogLevel level, const QString& text) const;
    void logText(const char* file, int lineNumber, Log::LogLevel level, const Log::LogCategory& category, const QString& text) const;
    void logHex(const char* file, int lineNumber, Log::LogLevel level, const QByteArray& data, const QString& tag = QString()) const;
    void logHex(const char* file, int lineNumber, Log::LogLevel level, const Log::LogCategory& category, const QByteArray& data, const QString& tag = QString()) const;

private:
    void commonInit();

    Log::LogCategory _baseCategory;
    Log::LogCategory _level1;
    Log::LogCategory _level2;
    Log::LogCategory _level3;
};

#endif // LOGGINGBASECLASS_H
