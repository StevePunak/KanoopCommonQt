#include "utility/abstractthreadclass.h"
#include "commonexception.h"

AbstractThreadClass::AbstractThreadClass(const Log::LogCategory &category, QObject *parent) :
    QObject(parent),
    LoggingBaseClass(category),
    _success(false),
    _stdout(stdout), _stderr(stderr)
{
    commonInit();
}

AbstractThreadClass::AbstractThreadClass(const QString &category, QObject *parent) :
    QObject(parent),
    LoggingBaseClass(category),
    _success(false),
    _stdout(stdout), _stderr(stderr)
{
    commonInit();
}

void AbstractThreadClass::commonInit()
{
    AbstractThreadClass::setObjectName(AbstractThreadClass::metaObject()->className());

    connect(&_thread, &QThread::started, this, &AbstractThreadClass::onThreadStarted);
    connect(&_thread, &QThread::finished, this, &AbstractThreadClass::onThreadFinished);
    moveToThread(&_thread);
}

AbstractThreadClass::~AbstractThreadClass()
{
    if(_thread.isRunning()) {
        try
        {
            _thread.quit();
            if(!_thread.wait(500)) {
                logText(LVL_DEBUG, "Thread never joined");
            }
            if(!_stopEvent.wait(500)) {
                logText(LVL_DEBUG, "Stop Event never fired");
            }
        }
        catch(...)
        {
            logText(LVL_DEBUG, "Thread termination exception");
        }
    }
    else {
        _stopEvent.set();
    }
}

bool AbstractThreadClass::start(const TimeSpan &timeout)
{
    bool result = true;
    if(_thread.isRunning()) {
        logText(LVL_WARNING, QString("%1: Tried to start while already running").arg(objectName()));
    }
    else {
        _thread.start();
        if(timeout != TimeSpan::zero() && _startEvent.wait(timeout) == false) {
            logText(LVL_ERROR, QString("%1: Thread start never completed").arg(objectName()));
            result = false;
        }
    }
    return result;
}

bool AbstractThreadClass::stop(const TimeSpan& timeout)
{
    bool result = true;
    if(_thread.isRunning() == false) {
        logText(LVL_WARNING, QString("%1: Tried to stop while not running").arg(objectName()));
    }
    else {
        _thread.quit();
        if(_stopEvent.wait(timeout) == false) {
            logText(LVL_ERROR, QString("%1: Thread stop never completed. Aborting thread!").arg(objectName()));
            _thread.terminate();
            result = false;
        }
    }
    return result;
}

void AbstractThreadClass::abort()
{
    logText(LVL_WARNING, QString("%1: Aborting thread").arg(objectName()));
    _thread.terminate();
    _thread.wait();
    logText(LVL_WARNING, QString("%1: Thread terminated").arg(objectName()));
}

bool AbstractThreadClass::waitForCompletion(const TimeSpan &timeout)
{
    bool result;
    if((result = _stopEvent.wait(timeout)) == false) {
        logText(LVL_WARNING, QString("%1: Thread never finished").arg(objectName()));
    }
    return result;
}

void AbstractThreadClass::finishAndStop(bool success, const QString &message)
{
    _success = success;
    _message = message;
    _thread.quit();
}

void AbstractThreadClass::onThreadStarted()
{
    try
    {
        threadStarted();
    }
    catch(const CommonException& e)
    {
        logText(LVL_ERROR, QString("%1: Thread start exception: %2").arg(objectName()).arg(e.message()));
    }
    _startEvent.set();
}

void AbstractThreadClass::onThreadFinished()
{
    try
    {
        threadFinished();
    }
    catch(const CommonException& e)
    {
        logText(LVL_ERROR, QString("%1: Thread start exception: %2").arg(objectName()).arg(e.message()));
    }
    _stopEvent.set();
}

