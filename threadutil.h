#ifndef THREADUTIL_H
#define THREADUTIL_H
#include <QString>
#include <QThread>
#include "kanoopcommon.h"

class KANOOP_EXPORT ThreadUtil
{
public:
    static QString currentThreadString() { return QString("0x%1").arg((quint64)QThread::currentThreadId(), 8, 16, QChar('0')); }
    static QString threadString(QThread* thread) { return QString("0x%1").arg((quint64)thread, 8, 16, QChar('0')); }
};

#endif // THREADUTIL_H
