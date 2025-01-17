/******************************************************************************************
**
** log.cpp
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Wed Oct 4 13:25:38 2023
**
******************************************************************************************/
#include "log.h"
#include "logcategoryprivate.h"
#include <Kanoop/datetimeutil.h>
#include <QDebug>
#include <QFileInfo>
#include <logcategory.h>
#ifndef WIN32
#include <syslog.h>
#endif

using namespace Log;

static Logger* _systemLog = nullptr;
const QList<QString> Log::Logger::_LevelStrings = {                       // clazy:exclude=non-pod-global-static
    "EMRG", "ALRT", "CRIT", "ERR", "WRN", "NOT", "INF", "DBG"
};

Logger::Logger() :
    _level(Debug),
    _flags(Standard),
    _identity("tess"),
    _logOpen(false),
    _stdout(stdout), _stderr(stderr)
{
}

void Logger::openLog()
{
    if(_flags & File) {
        openFile();
    }

    if(_flags & Syslog) {
        openSyslog();
    }

    _logOpen = true;
}

void Logger::logText(const char *file, int lineNumber, LogLevel level, const QString &text)
{
    logText(file, lineNumber, level, LogCategory(), text);
}

void Logger::logText(const char *file, int lineNumber, LogLevel level, const LogCategory &category, const QString &text)
{
    if(level > _level) {
        return;
    }

    LogCategoryPrivate* categoryPtr = nullptr;
    if(category.isValid()) {
        categoryPtr = _categories.value(category.name());
        if(categoryPtr != nullptr && level > categoryPtr->level()) {
            return;
        }
    }

    if(_logOpen == false) {
        openLog();
    }

    _writeLock.lock();

    QString buffer;
    QTextStream output(&buffer);
    if(_flags & Timestamp) {
        output << DateTimeUtil::currentToStandardString() << ' ';
    }
    if(_flags & LineNumbers) {
        output << QFileInfo(file).fileName().left(16).leftJustified(16, QChar(' ')) << ' ' << lineNumber << ' ';
    }
    if(_flags & Level && level <= Debug) {
        output << _LevelStrings[level] << ' ';
    }

    if(category.isValid()) {
        LogCategoryPrivate* categoryPtr = _categories.value(category.name());
        if(categoryPtr != nullptr) {
            output << '[' << categoryPtr->name() << ']' << ' ';
        }
    }

    output << text << Qt::endl;

    outputToDestinations(level, buffer);

    _writeLock.unlock();
}

void Logger::logHex(const char *file, int lineNumber, LogLevel level, const QByteArray &data, const QString &tag)
{
    logHex(file, lineNumber, level, LogCategory(), data, tag);
}

void Logger::logHex(const char *file, int lineNumber, LogLevel level, const LogCategory &category, const QByteArray &data, const QString &tag)
{
    if(level > _level) {
        return;
    }

    _writeLock.lock();

    QString buffer;
    QTextStream output(&buffer);
    if(_flags & Timestamp) {
        output << DateTimeUtil::currentToStandardString() << ' ';
    }
    if(_flags & LineNumbers) {
        output << QFileInfo(file).fileName() << ' ' << lineNumber << ' ';
    }
    if(_flags & Level && level <= Debug) {
        output << _LevelStrings[level] << ' ';
    }

    if(category.isValid()) {
        LogCategoryPrivate* categoryPtr = _categories.value(category.name());
        if(categoryPtr != nullptr) {
            output << '[' << categoryPtr->name() << ']' << ' ';
        }
    }

    if(tag.isEmpty() == false) {
        output << '[' << tag << ']';
    }

    if((_flags & (Timestamp | LineNumbers | Level)) || tag.isEmpty() == false) {
        output << Qt::endl;
    }

    QString hex = bufferToHex(data);
    output << hex << Qt::endl;

    outputToDestinations(level, buffer);

    _writeLock.unlock();
}

void Logger::enableOutputFlags(OutputFlags flags)
{
    setFlags(static_cast<OutputFlags>(_flags | flags));
}

void Logger::disableOutputFlags(OutputFlags flags)
{
    setFlags(static_cast<OutputFlags>(_flags & ~flags));
}

void Logger::setFilename(const QString &filename)
{
    _filename = filename;

    // if we have an open file, close it and reopen under new name
    if(_file.isOpen() && _file.fileName() != filename) {
        closeFile();
        openFile();
    }
}

void Logger::setIdentity(const QString &value)
{
    strncpy(_identity, qPrintable(value), sizeof(_identity) - 1);
    openSyslog();
}

LogCategory Logger::registerCategory(const QString &name)
{
    LogLevel level = LogCategory::DefaultLogLevel;

    _writeLock.lock();
    LogCategoryPrivate* category = _categories.value(name);
    if(category == nullptr) {
        QString parentName = LogCategory::parentName(name);
        if(parentName.isEmpty() == false) {
            LogCategoryPrivate* parent = _categories.value(parentName);
            if(parent != nullptr) {
                level = parent->level();
            }
        }
        category = LogCategoryPrivate::create(name, level);
        _categories.insert(name, category);
    }
    category->setLevel(level);
    _writeLock.unlock();
    return *category;
}

LogCategory Logger::registerCategory(const LogCategory &category)
{
    LogCategory newCategory = registerCategory(category.name());
    // TODO set the level
    return newCategory;
}

//LogCategory Logger::registerCategory(const LogCategory &category, LogLevel level)
//{
//    return registerCategory(category.name(), level);
//}

void Logger::setCategoryLevel(const QString &name, LogLevel level)
{
    LogCategory parent = registerCategory(name);
    _writeLock.lock();
    LogCategoryPrivate* ptr = _categories.value(name);
    ptr->setLevel(level);
    for(CategoryIndex::iterator it = _categories.begin();it !=_categories.end();it++) {
        LogCategoryPrivate* ptr = it.value();
        if(ptr->isChildOf(parent)) {
            ptr->setLevel(level);
        }
    }
    _writeLock.unlock();
}

void Logger::openFile()
{
    if(_file.isOpen() && _filename != _file.fileName()) {
        closeFile();
    }
    if(_file.isOpen() == false) {
        _file.setFileName(_filename);
        if(_file.open(QFile::WriteOnly) == false) {
            _stderr << "Failed to open log file at " << _filename << Qt::endl;
        }
    }
}

void Logger::closeFile()
{
    _file.close();
}

void Logger::openSyslog()
{
#ifndef WIN32
    closelog();
    openlog(_identity, LOG_CONS | LOG_NDELAY | LOG_PID, LOG_LOCAL1);
#endif
}

void Logger::closeSyslog()
{
#ifndef WIN32
    closelog();
#endif
}

void Logger::outputToDestinations(LogLevel level, const QString &text)
{
    if(_flags & Console) {
        if(level <= Error) {
            _stderr << text;
            _stderr.flush();
        }
        else {
            _stdout << text;
            _stdout.flush();
        }
    }

    if(_flags & File && _file.isOpen()) {
        _file.write(text.toUtf8());
    }

    if(_flags & QDebug) {
        qDebug() << text;
    }

    if(_flags & Syslog) {
#ifndef WIN32
        syslog(level, "%s", text.toLocal8Bit().constData());
#endif
    }
}

QString Logger::bufferToHex(const QByteArray &buffer)
{
    QString text;
    QTextStream output(&text);

    for(int index = 0;index < buffer.length();index += 16) {
        output << QString("%1: ").arg(index, 6, 16, QChar('0'));

        QString ascii;
        QTextStream asciiOutput(&ascii);
        int i;
        for(i = index;i < index + 16 && i < buffer.length();i++) {
            output << QString("%1").arg((quint8)buffer.constData()[i], 2, 16, QChar('0'));
            QChar character = buffer.constData()[i];
            if(character.isPrint()) {
                asciiOutput << character;
            }
            else {
                asciiOutput << '.';
            }
            if(i + 1 < index + 16 && i < buffer.length()) {
                output << ' ';
            }
        }

        for(;i < index + 16;i++) {
            output << "  ";
            if(i < index + 15) {
                output << ' ';
            }
        }
        output << "   " << ascii << Qt::endl;
    }
    return text;
}

Logger *Log::systemLog()
{
    static QMutex _lock;
    _lock.lock();
    if(_systemLog == nullptr) {
        _systemLog = new Logger;
        _systemLog->openLog();
    }
    _lock.unlock();
    return _systemLog;
}

void Log::logText(const char *file, int lineNumber, LogLevel level, const QString &text)
{
    systemLog()->logText(file, lineNumber, level, text);
}

void Log::logText(const char *file, int lineNumber, LogLevel level, const LogCategory &category, const QString &text)
{
    systemLog()->logText(file, lineNumber, level, category, text);
}

void Log::logHex(const char *file, int lineNumber, LogLevel level, const LogCategory &category, const QByteArray &data, const QString &tag)
{
    systemLog()->logHex(file, lineNumber, level, category, data, tag);
}

void Log::logHex(const char *file, int lineNumber, LogLevel level, const QByteArray &data, const QString &tag)
{
    systemLog()->logHex(file, lineNumber, level, data, tag);
}

LogLevel Log::level()
{
    return systemLog()->level();
}

void Log::setLevel(LogLevel value)
{
    systemLog()->setLevel(value);
}

OutputFlags Log::flags()
{
    return systemLog()->flags();
}

void Log::setFlags(OutputFlags flags)
{
    systemLog()->setFlags(flags);
}

QString Log::filename()
{
    return systemLog()->filename();
}

void Log::setFilename(const QString& filename)
{
    systemLog()->setFilename(filename);
}

QString Log::identity()
{
    return systemLog()->identity();
}

void Log::setIdentity(const QString& value)
{
    systemLog()->setIdentity(value);
}

void Log::enableOutputFlags(OutputFlags flags)
{
    systemLog()->enableOutputFlags(flags);
}

void Log::disableOutputFlags(OutputFlags flags)
{
    systemLog()->disableOutputFlags(flags);
}

LogCategory Log::registerCategory(const QString& name, LogLevel level)
{
    LogCategory result = systemLog()->registerCategory(name);
    systemLog()->setCategoryLevel(name, level);
    return result;
}

LogCategory Log::registerCategory(const QString& name)
{
    return systemLog()->registerCategory(name);
}

void Log::setCategoryLevel(const QString &name, LogLevel level)
{
    systemLog()->setCategoryLevel(name, level);
}

LogLevel Log::parseLevel(const QString &value, bool *parsed)
{
    bool success = false;
    LogLevel result = Debug;
    static const QMap<LogLevel, QStringList> levelMap =
    {
        { Emergency,            QStringList() << "emergency" << "emerg" << "emg" << "emrg"          },
        { Alert,                QStringList() << "alert" << "alrt"                                  },
        { Critical,             QStringList() << "critical" << "crit"                               },
        { Error,                QStringList() << "error" << "err"                                   },
        { Warning,              QStringList() << "warning" << "warn" << "wrn"                       },
        { Notice,               QStringList() << "notice" << "not"                                  },
        { Info,                 QStringList() << "informational" << "info" << "inf"                 },
        { Debug,                QStringList() << "debug" << "dbg"                                   },
    };

    bool isNumber;
    int parseNumber = value.toInt(&isNumber);
    if(isNumber && (parseNumber >= Emergency && parseNumber <= Debug)) {
        result = static_cast<LogLevel>(parseNumber);
        success = true;
    }
    else {
        for(QMap<LogLevel, QStringList>::const_iterator it = levelMap.constBegin();it != levelMap.constEnd();it++) {
            LogLevel level = it.key();
            const QStringList& strings = it.value();
            for(const QString& str : strings) {
                if(value.compare(str, Qt::CaseInsensitive) == 0) {
                    result = level;
                    success = true;
                    break;
                }
            }
        }
    }

    if(parsed != nullptr) {
        *parsed = success;
    }

    return result;
}
