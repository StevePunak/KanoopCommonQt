/**
 *  PathUtil
 *
 *  Some static helper methods for files. Much of the functionallity
 *  in here has been overtaken by the QFileInfo native Qt object.
 *
 *  However, the extremely useful combine() method will combine many
 *  strings into a well-formatted path name.
 *
 *  Stephen Punak, July 04 2019
 */
#ifndef PATHUTIL_H
#define PATHUTIL_H
#include <QString>
#include <QDir>
#include "kanoopcommon.h"

/**
 * @brief Static helper methods for filesystem path manipulation.
 */
class KANOOP_EXPORT PathUtil
{
public:
    /**
     * @brief Extract the basename from a file path.
     * @param path File path string
     * @return Filename portion of the path
     * @deprecated Prefer QFileInfo::baseName()
     */
    static QString fileName(const QString& path);

    /**
     * @brief Extract the directory portion from a file path.
     * @param path File path string
     * @return Directory portion of the path
     * @deprecated Prefer QFileInfo::filePath()
     */
    static QString dirName(const QString& path);

    /**
     * @brief Extract the file extension from a file path.
     * @param path File path string
     * @return Extension string (without leading dot)
     * @deprecated Prefer QFileInfo::suffix()
     */
    static QString extension(const QString& path);

    /**
     * @brief Remove a trailing directory separator from a path.
     * @param path Path string to trim
     * @return Path without trailing separator
     * @deprecated Prefer QFileInfo::suffix()
     */
    static QString trimTrailingSlash(const QString& path);

    /**
     * @brief Combine two path segments into a normalized path.
     * @param p1 First segment
     * @param p2 Second segment
     * @return Cleaned combined path
     */
    static QString combine(const QString& p1, const QString& p2)
    { return QDir::cleanPath(p1 + QDir::separator() + p2); }

    /**
     * @brief Combine three path segments into a normalized path.
     * @param p1 First segment
     * @param p2 Second segment
     * @param p3 Third segment
     * @return Cleaned combined path
     */
    static QString combine(const QString& p1, const QString& p2, const QString& p3)
    { return QDir::cleanPath(p1 + QDir::separator() + p2 + QDir::separator() + p3); }

    /**
     * @brief Combine four path segments into a normalized path.
     * @param p1 First segment
     * @param p2 Second segment
     * @param p3 Third segment
     * @param p4 Fourth segment
     * @return Cleaned combined path
     */
    static QString combine(const QString& p1, const QString& p2, const QString& p3, const QString& p4)
    { return QDir::cleanPath(p1 + QDir::separator() + p2 + QDir::separator() + p3 + QDir::separator() + p4); }

    /**
     * @brief Combine five path segments into a normalized path.
     * @param p1 First segment
     * @param p2 Second segment
     * @param p3 Third segment
     * @param p4 Fourth segment
     * @param p5 Fifth segment
     * @return Cleaned combined path
     */
    static QString combine(const QString& p1, const QString& p2, const QString& p3, const QString& p4, const QString& p5)
    { return QDir::cleanPath(p1 + QDir::separator() + p2 + QDir::separator() + p3 + QDir::separator() + p4 + QDir::separator() + p5); }

    /**
     * @brief Combine six path segments into a normalized path.
     * @param p1 First segment
     * @param p2 Second segment
     * @param p3 Third segment
     * @param p4 Fourth segment
     * @param p5 Fifth segment
     * @param p6 Sixth segment
     * @return Cleaned combined path
     */
    static QString combine(const QString& p1, const QString& p2, const QString& p3, const QString& p4, const QString& p5, const QString& p6)
    { return QDir::cleanPath(p1 + QDir::separator() + p2 + QDir::separator() + p3 + QDir::separator() + p4 + QDir::separator() + p5 + QDir::separator() + p6); }

    /**
     * @brief Combine seven path segments into a normalized path.
     * @param p1 First segment
     * @param p2 Second segment
     * @param p3 Third segment
     * @param p4 Fourth segment
     * @param p5 Fifth segment
     * @param p6 Sixth segment
     * @param p7 Seventh segment
     * @return Cleaned combined path
     */
    static QString combine(const QString& p1, const QString& p2, const QString& p3, const QString& p4, const QString& p5, const QString& p6, const QString& p7)
    { return QDir::cleanPath(p1 + QDir::separator() + p2 + QDir::separator() + p3 + QDir::separator() + p4 + QDir::separator() + p5 + QDir::separator() + p6 + QDir::separator() + p7); }

    /**
     * @brief Combine a list of path segments into a normalized path.
     * @param parts List of path segment strings
     * @return Cleaned combined path
     */
    static QString combine(const QStringList& parts);

    /**
     * @brief Find the first executable in PATH matching a basename.
     * @param basename Executable filename to search for
     * @return QFileInfo for the found executable, or an invalid QFileInfo if not found
     */
    static QFileInfo which(const QString& basename);

    /**
     * @brief Return the given directory with one level removed.
     * @param path Directory path to shorten
     * @return Parent directory path
     */
    static QString popLevel(const QString& path);
};

#endif // PATHUTIL_H
