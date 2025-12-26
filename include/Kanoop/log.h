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

#define LVL_EMERGENCY       __FILE__,__LINE__,Log::LogLevel::Emergency
#define LVL_ALERT           __FILE__,__LINE__,Log::LogLevel::Alert
#define LVL_CRITICAL        __FILE__,__LINE__,Log::LogLevel::Critical
#define LVL_ERROR           __FILE__,__LINE__,Log::LogLevel::Error
#define LVL_WARNING         __FILE__,__LINE__,Log::LogLevel::Warning
#define LVL_NOTICE          __FILE__,__LINE__,Log::LogLevel::Notice
#define LVL_INFO            __FILE__,__LINE__,Log::LogLevel::Info
#define LVL_DEBUG           __FILE__,__LINE__,Log::LogLevel::Debug


class LogConsumer;
namespace Log
{

class LogCategoryPrivate;

class LogCategory;

class KANOOP_EXPORT Logger
{
public:
    Logger();

    void openLog();
    void rotateLog(const QString &newFileName);

    void logText(const char* file, int lineNumber, LogLevel level, const QString& text);
    void logText(const char* file, int lineNumber, LogLevel level, const LogCategory& category, const QString& text);
    void logHex(const char* file, int lineNumber, LogLevel level, const QByteArray& data, const QString& tag = QString());
    void logHex(const char* file, int lineNumber, LogLevel level, const LogCategory& category, const QByteArray& data, const QString& tag = QString());

    LogLevel level() const { return _level; }
    void setLevel(LogLevel value) { _level = value; }

    OutputFlags flags() const { return _flags; }
    void setFlags(OutputFlags flags) { _flags = flags; }
    void enableOutputFlags(OutputFlags flags);
    void disableOutputFlags(OutputFlags flags);

    QString filename() const { return _filename; }
    void setFilename(const QString& filename);

    QString identity() const { return _identity; }
    void setIdentity(const QString& value);

    LogCategory registerCategory(const QString& name);
    LogCategory registerCategory(const LogCategory& category);
    QList<LogCategory> categories() const;
    void setCategoryLevel(const QString& name, LogLevel level);

    void addConsumer(LogConsumer* consumer);
    void removeConsumer(LogConsumer* consumer);

    bool isLogOpen() const { return _logOpen; }

private:
    class CategoryIndex : public QMap<QString, LogCategoryPrivate*>
    {
    };

    void openFile();
    void closeFile();

    void openSyslog();
    void closeSyslog();

    void outputToDestinations(LogLevel level, const LogCategory& category, const QDateTime& timestamp, const QString& formattedText, const QString& unformattedText);

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

KANOOP_EXPORT void logText(const char* file, int lineNumber, LogLevel level, const QString& text);
KANOOP_EXPORT void logText(const char* file, int lineNumber, LogLevel level, const LogCategory& category, const QString& text);
KANOOP_EXPORT void logHex(const char* file, int lineNumber, LogLevel level, const LogCategory& category, const QByteArray& data, const QString& tag = QString());
KANOOP_EXPORT void logHex(const char* file, int lineNumber, LogLevel level, const QByteArray& data, const QString& tag = QString());

KANOOP_EXPORT Logger* systemLog();

KANOOP_EXPORT LogLevel level();
KANOOP_EXPORT void setLevel(LogLevel value);

KANOOP_EXPORT OutputFlags flags();
KANOOP_EXPORT void setFlags(OutputFlags flags);

KANOOP_EXPORT QString filename();
KANOOP_EXPORT void setFilename(const QString& filename);

KANOOP_EXPORT QString identity();
KANOOP_EXPORT void setIdentity(const QString& value);

KANOOP_EXPORT void enableOutputFlags(OutputFlags flags);
KANOOP_EXPORT void disableOutputFlags(OutputFlags flags);

KANOOP_EXPORT LogCategory registerCategory(const QString& name);
KANOOP_EXPORT LogCategory registerCategory(const QString &name, LogLevel level);
KANOOP_EXPORT QList<LogCategory> categories();
KANOOP_EXPORT void setCategoryLevel(const QString& name, LogLevel level);

KANOOP_EXPORT void addConsumer(LogConsumer* consumer);
KANOOP_EXPORT void removeConsumer(LogConsumer* consumer);

KANOOP_EXPORT LogLevel parseLevel(const QString& value, bool* parsed = nullptr);

KANOOP_EXPORT QDateTime getLogStartTime(const QString& filename);
KANOOP_EXPORT QDateTime getLogEndTime(const QString& filename);


} // namespace log

#endif // LOG_H
