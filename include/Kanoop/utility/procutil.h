#ifndef PROCUTIL_H
#define PROCUTIL_H

#include <QStringList>
#include <Kanoop/kanoopcommon.h>

class KANOOP_EXPORT ProcUtil
{
public:
    static QStringList runningProcessNames();
    static QList<int> runningPids();
    static QString processNameFromPid(int pid);

private:
    static QString readProcStatusString(int pid, const QString& key);
    static QString readProcFileString(const QString& filename, const QString& key);
    static QStringList readProcCommandLine(int pid);
    static QString processNameFromCommandLine(const QString& commandLine);

    static const QString ProcDir;
    static const QString CmdLine;
};

#endif // PROCUTIL_H
