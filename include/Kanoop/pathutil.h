#ifndef PATHUTIL_H
#define PATHUTIL_H
#include <QString>
#include <QDir>
#include "kanoopcommon.h"

class KANOOP_EXPORT PathUtil
{
public:
    /**
     * @brief extract basename from given path
     * deprecated in favor of QFileInfo::basename
     */
    static QString fileName(const QString& path);

    /**
     * @brief extract directory from given path
     * deprecated in favor of QFileInfo::filePath
     */
    static QString dirName(const QString& path);

    /**
     * @brief extract extension from given path
     * deprecated in favor of QFileInfo::suffix
     */
    static QString extension(const QString& path);

    /**
     * @brief Trim slash from path if it exists
     * deprecated in favor of QFileInfo::suffix
     */
    static QString trimTrailingSlash(const QString& path);

    /**
     * @brief combine paths into a normalized path
     */
    static QString combine(const QString& p1, const QString& p2)
    { return QDir::cleanPath(p1 + QDir::separator() + p2); }
    static QString combine(const QString& p1, const QString& p2, const QString& p3)
    { return QDir::cleanPath(p1 + QDir::separator() + p2 + QDir::separator() + p3); }
    static QString combine(const QString& p1, const QString& p2, const QString& p3, const QString& p4)
    { return QDir::cleanPath(p1 + QDir::separator() + p2 + QDir::separator() + p3 + QDir::separator() + p4); }
    static QString combine(const QString& p1, const QString& p2, const QString& p3, const QString& p4, const QString& p5)
    { return QDir::cleanPath(p1 + QDir::separator() + p2 + QDir::separator() + p3 + QDir::separator() + p4 + QDir::separator() + p5); }
    static QString combine(const QString& p1, const QString& p2, const QString& p3, const QString& p4, const QString& p5, const QString& p6)
    { return QDir::cleanPath(p1 + QDir::separator() + p2 + QDir::separator() + p3 + QDir::separator() + p4 + QDir::separator() + p5 + QDir::separator() + p6); }
    static QString combine(const QString& p1, const QString& p2, const QString& p3, const QString& p4, const QString& p5, const QString& p6, const QString& p7)
    { return QDir::cleanPath(p1 + QDir::separator() + p2 + QDir::separator() + p3 + QDir::separator() + p4 + QDir::separator() + p5 + QDir::separator() + p6 + QDir::separator() + p7); }
    static QString combine(const QStringList& parts);

    /**
     * @brief Find first executable in path matching basename
     * @param executable file basename
     * @return
     */
    static QFileInfo which(const QString& basename);

    /**
     * @brief Return the given directory with one level popped
     */
    static QString popLevel(const QString& path);
};

#endif // PATHUTIL_H
