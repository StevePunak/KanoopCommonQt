#include <stdarg.h>
#include "klog.h"
#include <QDateTime>
#include "pathutil.h"

const char *KLog::_levelStrings[] =
    {
        "ALW", "DBG", "INF", "WRN", "ERR", "FAT", "NOT"
    };

KLog* KLog::_systemLog = nullptr;
KLog::OutputFlags KLog::_defaultOutputFlags = KLog::OutputFlags::Standard;
KLog::LogLevel KLog::_defaultLogLevel = KLog::LogLevel::Debug;

KLog::KLog() :
    _outputFlags(_defaultOutputFlags),
    _level(_defaultLogLevel),
    _stdout(stdout),
    _file(nullptr),
    _verbosity(0)
{
}

void KLog::logText(const char *file, int line, KLog::LogLevel level, const char *format, ...)
{
    va_list vargs;
    va_start(vargs, format);

    char outputBuffer[0xffff];

    vsnprintf(outputBuffer, sizeof(outputBuffer), format, vargs);
    va_end(vargs);

    QString output;
    QTextStream text(&output);

    if(_outputFlags & OutputFlags::Timestamp)
        writeTimestamp(text);

    if(_outputFlags & OutputFlags::LineNumbers)
    {
        text << PathUtil::fileName(file).leftJustified(12, ' ', true) << ' ';
        text << QString::number(line).leftJustified(4, ' ', true) << ' ';
    }

    if(_outputFlags & OutputFlags::Level && level <= LogLevel::Nothing)
        text << _levelStrings[level] << ' ';

    text << outputBuffer;

    QString final = output;
    outputToDestinations(final);
}

void KLog::logHex(const unsigned char *data, int count)
{
    QString output;
    output.reserve(count * 3 + count + 30);
    QTextStream text(&output);
    char outBuffer[128];
    char dotBuffer[128];
    int lineOffset = 0;
    for(int x = 0;x < count;x++)
    {
        lineOffset = x % 16;
        if(lineOffset == 0)
        {
            snprintf(outBuffer, sizeof(outBuffer), "%06x: ", x);
            text << outBuffer;
            memset(dotBuffer, 0, sizeof(dotBuffer));
        }
        snprintf(outBuffer, sizeof(outBuffer), "%02X ", data[x]);
        text << outBuffer;

        dotBuffer[lineOffset] = QChar::isPrint(data[x]) ? data[x] : '.';
        if(lineOffset == 15)
        {
            text << "  " << dotBuffer << endl;
        }
    }
    if(lineOffset < 15)
    {
        for(++lineOffset;lineOffset < 16;lineOffset++)
            text << "   ";
        text << "  " << dotBuffer << endl;
    }
    outputToDestinations(output);
}

void KLog::setLogFile(const QString &fileName)
{
    _fileName = fileName;
    _file = new QFile(fileName);
    _file->open(QIODevice::Append | QIODevice::WriteOnly);
    if(_file->isOpen())
    {
        _fileOutputStream = new QTextStream(_file);
        _outputFlags = (OutputFlags)(_outputFlags | OutputFlags::File);
    }
    else
    {
        qDebug() << "Could not open file " << fileName << " " << _file->errorString();
    }
}

void KLog::sysLogText(const char *file, int line, KLog::LogLevel level, const char *format, ...)
{
    va_list vargs;
    va_start(vargs, format);

    char outputBuffer[0xffff];

    vsnprintf(outputBuffer, sizeof(outputBuffer), format, vargs);
    va_end(vargs);

    systemLog()->logText(file, line, level, outputBuffer);
}

void KLog::sysLogText(const char *file, int line, KLog::LogLevel level, const QString &output)
{
    char buffer[32768];
    size_t count = qMin((size_t)output.length(), sizeof(buffer) - 1);
    memcpy(buffer, output.toLocal8Bit().constData(), count);
    buffer[count] = 0;
    systemLog()->logText(file, line, level, buffer);
}

void KLog::outputToDestinations(QString text)
{
    _outputLock.lock();

    if(_outputFlags & OutputFlags::File)
    {
        *_fileOutputStream << text << endl;
        _fileOutputStream->flush();
    }
    if(_outputFlags & OutputFlags::Console)
    {
        _stdout << text << endl;
    }
    if(_outputFlags & OutputFlags::QDebug)
    {
        qDebug() << text;
    }

    _outputLock.unlock();
}

void KLog::writeTimestamp(QTextStream& output)
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    QString formatted = QString().asprintf("%04d-%02d-%02d %02d:%02d:%02d.%03d",
                                          now.date().year(), now.date().month(), now.date().day(),
                                          now.time().hour(), now.time().minute(), now.time().second(), now.time().msec());
    output << formatted << ' ';
}

void KLog::sysLogHex(const unsigned char *data, int count)
{
    systemLog()->logHex(data, count);
}

void KLog::setSystemLogFile(const QString &fileName)
{
    systemLog()->setLogFile(fileName);
}

void KLog::openSystemLog()
{
    _systemLog = new KLog();
}

KLog *KLog::systemLog()
{
    if(_systemLog == nullptr)
    {
        openSystemLog();
    }
    return _systemLog;
}


