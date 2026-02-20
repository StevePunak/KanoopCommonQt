/**
 *  LockingQueue
 *
 *  Thread-safe Multi-producer Multi-consumer blocking queue
 *
 *  Stephen Punak, July 08 2019
 */
#ifndef LOCKINGQUEUE_H
#define LOCKINGQUEUE_H

#include <QList>
#include <QWaitCondition>
#include <QMutex>

template <class T>
class LockingQueue : public QList<T>
{
public:
    T dequeue(quint32 waitTimeMs, bool& success)
    {
        T result = T();
        success = false;

        _queueLock.lock();              // don't allow anyone to add until we check the count

        if(this->count() > 0)
        {
            result = this->takeFirst();
            success = true;
            _queueLock.unlock();
        }
        else
        {
            if(_condition.wait(&_queueLock, quint32(waitTimeMs)))
            {
                if(this->count() > 0)
                {
                    result = this->takeFirst();
                    success = true;
                }
            }
            _queueLock.unlock();
        }
        return result;
    }

    void enqueue(const T& t)
    {
        _queueLock.lock();
        this->append(t);
        _queueLock.unlock();
        _condition.notify_one();
    }

private:
    QWaitCondition _condition;
    QMutex _queueLock;
};

#endif // LOCKINGQUEUE_H
