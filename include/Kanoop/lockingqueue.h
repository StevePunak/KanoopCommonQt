/**
 *  LockingQueue
 *
 *  Blocking multi-producer multi-consumer queue
 *
 *  Stephen Punak, July 08 2019
 */
#ifndef LOCKINGQUEUE_H
#define LOCKINGQUEUE_H

#include <QDeadlineTimer>
#include <QList>
#include <QWaitCondition>
#include <QMutex>

/**
 * @brief Blocking multi-producer multi-consumer queue.
 *
 * ⚠ Every accessor must take _queueLock before touching _items, which is why the
 * container is a private member rather than a base class.
 * ⚠ _queueLock is not recursive: no method here may call another method of this
 * class while holding it.
 *
 * @tparam T Element type stored in the queue
 */
template <class T>
class LockingQueue
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

        QDeadlineTimer deadline(waitTimeMs);

        _queueLock.lock();              // don't allow anyone to add until we check the count
        while(true)
        {
            if(_items.count() > 0)
            {
                result = _items.takeFirst();
                success = true;
                break;
            }
            if(_condition.wait(&_queueLock, deadline) == false)
            {
                break;
            }
        }
        _queueLock.unlock();
        return result;
    }

    /**
     * @brief Append an element to the back of the queue and wake one waiting consumer.
     * @param t Element to enqueue
     */
    void enqueue(const T& t)
    {
        _queueLock.lock();
        _items.append(t);
        _queueLock.unlock();
        _condition.notify_one();
    }

    /**
     * @brief Number of elements currently queued.
     * @return Element count
     */
    qsizetype count() const
    {
        QMutexLocker locker(&_queueLock);
        return _items.count();
    }

    /**
     * @brief Whether the queue currently holds no elements.
     * @return true when empty
     */
    bool isEmpty() const
    {
        QMutexLocker locker(&_queueLock);
        return _items.isEmpty();
    }

    /**
     * @brief Discard every queued element.
     */
    void clear()
    {
        QMutexLocker locker(&_queueLock);
        _items.clear();
    }

private:
    QList<T> _items;
    QWaitCondition _condition;
    mutable QMutex _queueLock;
};

#endif // LOCKINGQUEUE_H
