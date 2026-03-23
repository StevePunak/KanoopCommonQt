#ifndef PROCFSUTIL_H
#define PROCFSUTIL_H
#include <Qt>

class ProcFsUtil
{
public:
    static QList<int> runningPids();
    static QList<int> pidsForProcessName(const QString& processName);
    static uint64_t getVmRSS(int pid);
    static uint64_t getUnevictable();
    static QString getProcessName(int pid);

    static QStringList getProcessMemoryUsageValues(int pid = 0);

private:
    static QString getProcStatusString(int pid, const QString& key);
    static QString getMemInfoString(const QString& key);
    static QString getProcFileString(const QString& filename, const QString& key);
    static uint64_t toUInt64(const QString& value);
};

#endif // PROCFSUTIL_H
