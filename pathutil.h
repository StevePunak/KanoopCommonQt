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
    static QString combine(const QString& p1, const QString& p2) { return QDir::cleanPath(p1 + '/' + p2); }
    static QString combine(const QString& p1, const QString& p2, const QString& p3) { return QDir::cleanPath(p1 + '/' + p2 + '/' + p3); }
    static QString combine(const QString& p1, const QString& p2, const QString& p3, const QString& p4) { return QDir::cleanPath(p1 + '/' + p2 + '/' + p3 + '/' + p4); }
    static QString combine(const QString& p1, const QString& p2, const QString& p3, const QString& p4, const QString& p5) { return QDir::cleanPath(p1 + '/' + p2 + '/' + p3 + '/' + p4 + '/' + p5); }
    static QString combine(const QStringList& parts);

    /**
     * @brief Find first executable in path matching basename
     * @param executable file basename
     * @return
     */
    static QFileInfo which(const QString& basename);
};

#endif // PATHUTIL_H
