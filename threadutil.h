#ifndef THREADUTIL_H
#define THREADUTIL_H
#include <QString>
#include <QThread>

class ThreadUtil
{
public:
    static QString currentThreadString() { return QString("%1").arg((quint64)QThread::currentThreadId(), 8, 16, QChar('0')); }
};

#endif // THREADUTIL_H
