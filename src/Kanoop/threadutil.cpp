#include "Kanoop/threadutil.h"

QString ThreadUtil::currentThreadString()
{
    return threadString(QThread::currentThread());
}

QString ThreadUtil::threadString(QThread* thread)
{
    char buf[1024];
    snprintf(buf, sizeof(buf), "%p", thread);
    return buf;
}
