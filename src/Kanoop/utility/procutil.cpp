#include <QProcess>
#include <QDebug>
#include <QFileInfoList>
#include <QDir>
#include <pathutil.h>
#include <fileutil.h>

#ifdef Q_OS_WIN
#include <windows.h>
#include <tlhelp32.h>
#endif
#include "utility/procutil.h"

const QString ProcUtil::ProcDir     = "/proc";
const QString ProcUtil::CmdLine     = "cmdline";

QStringList ProcUtil::runningProcessNames()
{
    QStringList processList;
#ifdef Q_OS_UNIX
    QList<int> pids = runningPids();
    for(int pid : pids) {
        QStringList args = readProcCommandLine(pid);
        if(args.count() > 0) {
            QString name = processNameFromCommandLine(args.at(0));
            if(name.isEmpty() == false) {
                processList.append(name);
            }
        }
    }
#elif defined(Q_OS_WIN)
    HANDLE handle;
    PROCESSENTRY32 pe32;
    handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (handle == INVALID_HANDLE_VALUE) {
        return processList;
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(handle, &pe32)) {
        CloseHandle(handle);
        return processList;
    }
    do {
        QString processName = QString::fromWCharArray(pe32.szExeFile);
        processList.append(processName);
    } while (Process32Next(handle, &pe32));
    CloseHandle(handle);
#endif
    return processList;
}

QList<int> ProcUtil::runningPids()
{
    // TODO Windows

    QList<int> result;
    QFileInfoList fileInfos = QDir(ProcDir).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(const QFileInfo& fileInfo : fileInfos) {
        bool parsed;
        int pid = fileInfo.fileName().toInt(&parsed);
        if(parsed) {
            result.append(pid);
        }
    }
    return result;
}

QString ProcUtil::processNameFromPid(int pid)
{
    // TODO Windows

    QString result = readProcStatusString(pid, "Name");
    if(result.startsWith("MSys")) {
        qWarning() << "herhe";
    }
    return result;
}

QString ProcUtil::readProcStatusString(int pid, const QString& key)
{
    QString filename = PathUtil::combine(ProcDir, QString::number(pid), "status");
    QString value = readProcFileString(filename, key);
    return value;
}

QString ProcUtil::readProcFileString(const QString& filename, const QString& key)
{
    QString result;
    QStringList lines;
    if(FileUtil::readAllLines(filename, lines) == false) {
        return result;
    }

    QString needle = key + ':';
    for(QString& line : lines) {
        if(line.startsWith(needle)) {
            result = line.remove(0, needle.length()).trimmed();
            break;
        }
    }
    return result;
}

QStringList ProcUtil::readProcCommandLine(int pid)
{
    QStringList result;
    QString path = PathUtil::combine(ProcDir, QString::number(pid), CmdLine);
    QByteArray contents;
    if(FileUtil::readAllBytes(path, contents) == true) {
        result = QString(contents).split('\0');
    }
    return result;
}

QString ProcUtil::processNameFromCommandLine(const QString& commandLine)
{
    QStringList parts = commandLine.split(QDir::separator());
    return parts.last();
}
