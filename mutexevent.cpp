#include "mutexevent.h"


void MutexEvent::set()
{
    _condition.notify_one();
}

bool MutexEvent::wait(int msecs)
{
    QMutex mutex;
    mutex.lock();
    bool result = _condition.wait(&mutex, msecs);
    mutex.unlock();
    return result;
}
