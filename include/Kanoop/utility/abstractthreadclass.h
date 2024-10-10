#ifndef ABSTRACTTHREADCLASS_H
#define ABSTRACTTHREADCLASS_H

#include <QMutex>
#include <QObject>
#include <QTextStream>
#include <QThread>

#include <Kanoop/mutexevent.h>
#include <Kanoop/log.h>
#include <Kanoop/kanoopcommon.h>

#include "loggingbaseclass.h"

class KANOOP_EXPORT AbstractThreadClass : public QObject,
                            public LoggingBaseClass
{
    Q_OBJECT

public:
    AbstractThreadClass(const Log::LogCategory& category = Log::LogCategory(), QObject* parent = nullptr);
    AbstractThreadClass(const QString& category, QObject* parent = nullptr);
    virtual ~AbstractThreadClass();

    virtual bool start(const TimeSpan& timeout = TimeSpan::zero());
    virtual bool stop(const TimeSpan &timeout = TimeSpan::zero());

    virtual void abort();

    bool waitForCompletion(const TimeSpan& timeout = TimeSpan::zero());

    bool success() const { return _success; }
    QString completionMessage() const { return _message; }

protected:
    virtual void threadStarted() = 0;
    virtual void threadFinished() {}

    void finishAndStop(bool success, const QString& message = QString());

    void writeLine(const QString& line) { _stdout << line << Qt::endl; }
    void writeErrorLine(const QString& line) { _stderr << line << Qt::endl; }

private:
    void commonInit();

    bool _success;
    QString _message;

    QThread _thread;
    QTextStream _stdout;
    QTextStream _stderr;

    MutexEvent _startEvent;
    MutexEvent _stopEvent;

signals:
    void started();
    void finished();

private slots:
    void onThreadStarted();
    void onThreadFinished();
};

#endif // ABSTRACTTHREADCLASS_H
