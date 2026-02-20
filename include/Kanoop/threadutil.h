/**
 *  ThreadHelper
 *
 *  Some static helper methods for create a string describing threads.
 *
 *  Stephen Punak, July 08 2019
 */
#ifndef THREADUTIL_H
#define THREADUTIL_H
#include <QString>
#include <QThread>
#include "kanoopcommon.h"

/**
 * @brief Static helper methods for obtaining descriptive strings about QThread objects.
 */
class KANOOP_EXPORT ThreadUtil
{
public:
    /**
     * @brief Return a human-readable description of the currently executing thread.
     * @return String identifying the current thread (e.g. address and name)
     */
    static QString currentThreadString();

    /**
     * @brief Return a human-readable description of a specific thread.
     * @param thread Pointer to the QThread to describe
     * @return String identifying the thread (e.g. address and name)
     */
    static QString threadString(QThread* thread);

    /**
     * @brief Spin-wait (busy-loop) for the specified number of microseconds.
     * @param usecs Duration to wait in microseconds
     */
    static inline void spinSleep(uint64_t usecs);
};

#endif // THREADUTIL_H
