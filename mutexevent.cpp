#include "mutexevent.h"
#include "klog.h"

MutexEvent::MutexEvent() :
    _isset(false),
    _debug(false)
{
}


void MutexEvent::set()
{
    if(_debug)
        KLog::sysLogText(KLOG_DEBUG, "setting event 1");
    _checkMutex.lock();
    if(_debug)
        KLog::sysLogText(KLOG_DEBUG, "setting event 2");
    _isset = true;
    _condition.notify_one();
    _checkMutex.unlock();
    if(_debug)
        KLog::sysLogText(KLOG_DEBUG, "setting event 3");
}

void MutexEvent::clear()
{
    if(_debug)
        KLog::sysLogText(KLOG_DEBUG, "clear event");
    _checkMutex.lock();
    _isset = false;
    _checkMutex.unlock();
}

bool MutexEvent::wait(int msecs)
{
    if(_debug)
        KLog::sysLogText(KLOG_DEBUG, "wait event %d", msecs);
    _checkMutex.lock();
    bool result = _isset;

    if(_isset == false)
    {
        if(_debug)
            KLog::sysLogText(KLOG_DEBUG, "wait event set is false");
        QMutex mutex;
        mutex.lock();
        _checkMutex.unlock();

        result = _condition.wait(&mutex, msecs);

        _checkMutex.lock();
        _isset = false;
        _checkMutex.unlock();

        if(_debug)
            KLog::sysLogText(KLOG_DEBUG, "wait event complete");
        mutex.unlock();
    }
    else
    {
        if(_debug)
            KLog::sysLogText(KLOG_DEBUG, "wait event fast return");
        _checkMutex.unlock();
    }
    return result;
}
