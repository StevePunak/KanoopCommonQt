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

class KANOOP_EXPORT ThreadUtil
{
public:
    static QString currentThreadString();
    static QString threadString(QThread* thread);
    static inline void spinSleep(uint64_t usecs);
};

#endif // THREADUTIL_H
