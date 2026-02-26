/**
 * @brief Static utilities for querying running Linux processes via /proc.
 */
#ifndef PROCUTIL_H
#define PROCUTIL_H

#include <QStringList>
#include <Kanoop/kanoopcommon.h>

/**
 * @brief Static helper methods for enumerating and inspecting running processes.
 *
 * Reads process information from the Linux /proc filesystem.
 */
class KANOOP_EXPORT ProcUtil
{
public:
    /**
     * @brief Return the names of all running processes.
     * @return List of process name strings
     */
    static QStringList runningProcessNames();

    /**
     * @brief Return the PIDs of all running processes.
     * @return List of process ID integers
     */
    static QList<int> runningPids();

    /**
     * @brief Return the process name for a given PID.
     * @param pid Process ID to look up
     * @return Process name string, or empty string if not found
     */
    static QString processNameFromPid(int pid);

private:
    /** @brief Read a key-value field from a process's /proc/<pid>/status file. */
    static QString readProcStatusString(int pid, const QString& key);
    /** @brief Read a key-value field from a proc filesystem file. */
    static QString readProcFileString(const QString& filename, const QString& key);
    /** @brief Read the command-line tokens of a process. */
    static QStringList readProcCommandLine(int pid);
    /** @brief Extract the process name from a command-line string. */
    static QString processNameFromCommandLine(const QString& commandLine);

    static const QString ProcDir;
    static const QString CmdLine;
};

#endif // PROCUTIL_H
