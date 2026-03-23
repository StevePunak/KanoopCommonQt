#include "utility/procfsutil.h"

#include <Kanoop/log.h>
#include <commonexception.h>

#include <Kanoop/fileutil.h>
#include <Kanoop/pathutil.h>

#include <QCoreApplication>

QList<int> ProcFsUtil::runningPids()
{
    QList<int> pids;
    QFileInfoList dirs = QDir("/proc").entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(const QFileInfo& dir : dirs) {
        bool ok;
        int pid = dir.fileName().toInt(&ok);
        if(ok) {
            pids.append(pid);
        }
    }
    return pids;
}

QList<int> ProcFsUtil::pidsForProcessName(const QString &processName)
{
    QList<int> pidsForProcess;
    QList<int> allPids = runningPids();
    for(int pid : allPids) {
        QString val = getProcStatusString(pid, "Name");
        if(val == processName) {
            pidsForProcess.append(pid);
        }
    }
    return pidsForProcess;
}

uint64_t ProcFsUtil::getVmRSS(int pid)
{
    uint64_t result = 0;
    QString val = getProcStatusString(pid, "VmRSS");
    if(val.isEmpty() == false) {
        result = toUInt64(val);
    }
    return result;
}

uint64_t ProcFsUtil::getUnevictable()
{
    uint64_t result = 0;
    QString val = getMemInfoString("Unevictable");
    if(val.isEmpty() == false) {
        result = toUInt64(val);
    }
    return result;
}

QString ProcFsUtil::getProcessName(int pid)
{
    QString result = getProcStatusString(pid, "Name");
    return result;
}

QStringList ProcFsUtil::getProcessMemoryUsageValues(int pid)
{
    QStringList result;
    try
    {
        if(pid == 0) {
            pid = QCoreApplication::instance()->applicationPid();
        }

        QString path = PathUtil::combine("/proc", QString::number(pid), "status");
        QStringList lines;
        if(FileUtil::readAllLines(path, lines) == false) {
            throw CommonException(QString("Failed to open %1").arg(path));
        }

        for(const QString& line : lines) {
            if(line.startsWith("Vm")) {
                result.append(line);
            }
        }
    }
    catch(const CommonException& e)
    {
        Log::logText(LVL_WARNING, QString("Failed to get mem usage: %1").arg(e.message()));
    }
    return result;
}

QString ProcFsUtil::getProcStatusString(int pid, const QString &key)
{
    QString path = PathUtil::combine("/proc", QString::number(pid), "status");
    QString result = getProcFileString(path, key);
    return result;
}

QString ProcFsUtil::getMemInfoString(const QString &key)
{
    QString result = getProcFileString("/proc/meminfo", key);
    return result;
}

QString ProcFsUtil::getProcFileString(const QString &filename, const QString &key)
{
    QString result;
    QStringList lines;
    if(FileUtil::readAllLines(filename, lines)) {
        for(const QString& line : lines) {
            if(line.startsWith(key)) {
                int index = line.indexOf(':');
                if(index > 0 && line.length() > index + 1) {
                    result = line.mid(index + 1).trimmed();
                    break;
                }
            }
        }
    }
    return result;
}

uint64_t ProcFsUtil::toUInt64(const QString &value)
{
    uint64_t result = 0;
    QStringList parts = value.split(' ', Qt::SkipEmptyParts);
    if(parts.count() > 0) {
        result = parts.at(0).toULong();
        if(parts.count() > 1) {
            QString mult = parts.at(1).toLower();
            if(mult.startsWith("k")) {
                result *= 1000;
            }
            else if(mult.startsWith("m")) {
                result *= 1000000;
            }
            else if(mult.startsWith("g")) {
                result *= 1000000000;
            }
        }
    }
    return result;
}
