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
    _stdout(stdout)
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

    qDebug() << output;

}

void KLog::sysLogText(const char *file, int line, KLog::LogLevel level, const char *format, ...)
{
    if(_systemLog == nullptr)
        _systemLog = new KLog();

    va_list vargs;
    va_start(vargs, format);

    char outputBuffer[0xffff];

    vsnprintf(outputBuffer, sizeof(outputBuffer), format, vargs);
    va_end(vargs);

    _systemLog->logText(file, line, level, outputBuffer);
}

void KLog::outputToDestinations(QString& text)
{
    if(_outputFlags & OutputFlags::File)
    {
        // todo
    }
    if(_outputFlags & OutputFlags::Console)
    {
        _stdout << text << endl;
    }
    if(_outputFlags & OutputFlags::QDebug)
    {
        qDebug() << text;
    }
}

void KLog::writeTimestamp(QTextStream& output)
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    QString formatted = QString().sprintf("%04d-%02d-%02d %02d:%02d:%02d.%03d",
                                          now.date().year(), now.date().month(), now.date().day(),
                                          now.time().hour(), now.time().minute(), now.time().second(), now.time().msec());
    output << formatted << ' ';
}

void KLog::sysLogHex(const unsigned char *data, int count)
{
    QString output;
    QTextStream text(&output);
    for(int x = 0;x < count;x++)
    {
        char outByte[10];
        snprintf(outByte, sizeof(outByte), "%02X ", data[x]);
        text << outByte;
    }
    qDebug() << output;
}

void KLog::openSystemLog()
{
    _systemLog = new KLog();
}


