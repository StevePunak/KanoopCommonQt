#include "Kanoop/mutexevent.h"
#include "Kanoop/log.h"

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
    _isWaiting = true;

    if(_isset == false)
    {
        if(msecs > 0)
            result = _condition.wait(&_checkMutex, msecs);
        else
            result = _condition.wait(&_checkMutex);

        _isset = false;
        _isWaiting = false;
        _checkMutex.unlock();
    }
    else
    {
        _isWaiting = false;
        _isset = false;
        _checkMutex.unlock();
    }
    return result;
}
