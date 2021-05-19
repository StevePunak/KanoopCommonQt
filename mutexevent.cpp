#include "mutexevent.h"
#include "klog.h"

void MutexEvent::set()
{
    _checkMutex.lock();
    _isset = true;
    if(!_wakeAllWaiters)
        _condition.notify_one();
    else
        _condition.notify_all();
    _checkMutex.unlock();
}

void MutexEvent::clear()
{
    _checkMutex.lock();
    _isset = false;
    _checkMutex.unlock();
}

bool MutexEvent::wait(int msecs)
{
    _checkMutex.lock();
    bool result = _isset;

    if(_isset == false)
    {
        QMutex mutex;
        mutex.lock();
        _checkMutex.unlock();

        result = _condition.wait(&mutex, msecs);

        _checkMutex.lock();
        _isset = false;
        _checkMutex.unlock();

        mutex.unlock();
    }
    else
    {
        _checkMutex.unlock();
    }
    return result;
}
