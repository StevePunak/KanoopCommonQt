/******************************************************************************************
**
** log.h
**
** Copyright (C) Tesseract Engineering, Inc - All Rights Reserved
**
** This source code is protected under international copyright law.  All rights
** reserved and protected by the copyright holders.
**
** This file is confidential and only available to authorized individuals with the
** permission of the copyright holders.  If you encounter this file and do not have
** permission, please contact the copyright holders and delete this file.
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
#include <syslog.h>
#include "loggingtypes.h"

#define LVL_EMERGENCY       __FILE__,__LINE__,Log::LogLevel::Emergency
#define LVL_ALERT           __FILE__,__LINE__,Log::LogLevel::Alert
#define LVL_CRITICAL        __FILE__,__LINE__,Log::LogLevel::Critical
#define LVL_ERROR           __FILE__,__LINE__,Log::LogLevel::Error
#define LVL_WARNING         __FILE__,__LINE__,Log::LogLevel::Warning
#define LVL_NOTICE          __FILE__,__LINE__,Log::LogLevel::Notice
#define LVL_INFO            __FILE__,__LINE__,Log::LogLevel::Info
#define LVL_DEBUG           __FILE__,__LINE__,Log::LogLevel::Debug

namespace Log
{

class LogCategoryPrivate;

class LogCategory;

class Logger
{
public:
    Logger();

    void openLog();

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
//    LogCategory registerCategory(const LogCategory& category, LogLevel level);
    void setCategoryLevel(const QString& name, LogLevel level);

    bool isLogOpen() const { return _logOpen; }

private:
    class CategoryIndex : public QMap<QString, LogCategoryPrivate*>
    {
    };

    void openFile();
    void closeFile();

    void openSyslog();
    void closeSyslog();

    void outputToDestinations(LogLevel level, const QString& text);

    static QString bufferToHex(const QByteArray& buffer);

    LogLevel _level;
    OutputFlags _flags;
    QString _filename;
    char _identity[512];

    bool _logOpen;

    QTextStream _stdout;
    QTextStream _stderr;

    QFile _file;
    QMutex _writeLock;

    CategoryIndex _categories;

    static const QList<QString> _LevelStrings;
};

void logText(const char* file, int lineNumber, LogLevel level, const QString& text);
void logText(const char* file, int lineNumber, LogLevel level, const LogCategory& category, const QString& text);
void logHex(const char* file, int lineNumber, LogLevel level, const LogCategory& category, const QByteArray& data, const QString& tag = QString());
void logHex(const char* file, int lineNumber, LogLevel level, const QByteArray& data, const QString& tag = QString());

Logger* systemLog();

LogLevel level();
void setLevel(LogLevel value);

OutputFlags flags();
void setFlags(OutputFlags flags);

QString filename();
void setFilename(const QString& filename);

QString identity();
void setIdentity(const QString& value);

void enableOutputFlags(OutputFlags flags);
void disableOutputFlags(OutputFlags flags);

LogCategory registerCategory(const QString& name);
LogCategory registerCategory(const QString &name, LogLevel level);
void setCategoryLevel(const QString& name, LogLevel level);

LogLevel parseLevel(const QString& value, bool* parsed = nullptr);

} // namespace log

#endif // LOG_H
