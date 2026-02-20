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

/**
 * @brief Thread-safe, blocking multi-producer multi-consumer queue.
 *
 * @tparam T Element type stored in the queue
 */
template <class T>
class LockingQueue : public QList<T>
{
public:
    /**
     * @brief Remove and return the front element, blocking until one is available or timeout elapses.
     * @param waitTimeMs Maximum time to wait in milliseconds
     * @param success Output set to true if an element was dequeued, false on timeout
     * @return Dequeued element, or a default-constructed T on timeout
     */
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

    /**
     * @brief Append an element to the back of the queue and wake one waiting consumer.
     * @param t Element to enqueue
     */
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
