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

void ThreadUtil::spinSleep(uint64_t usecs)
{
    std::chrono::time_point start = std::chrono::high_resolution_clock::now();
    while(true) {
        std::chrono::time_point now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
        if(duration.count() >= (int64_t)usecs) {
            break;
        }
    }
}
