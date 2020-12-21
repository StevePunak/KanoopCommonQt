#ifndef PATHUTIL_H
#define PATHUTIL_H
#include <QString>
#include <QDir>

class PathUtil
{
public:
    static QString fileName(const QString& path);
    static QString dirName(const QString& path);
    static QString trimTrailingSlash(const QString& path);
    static QString combine(const QString& p1, const QString& p2) { return QDir::cleanPath(p1 + '/' + p2); }
    static QString combine(const QString& p1, const QString& p2, const QString& p3) { return QDir::cleanPath(p1 + '/' + p2 + '/' + p3); }
    static QString combine(const QString& p1, const QString& p2, const QString& p3, const QString& p4) { return QDir::cleanPath(p1 + '/' + p2 + '/' + p3 + '/' + p4); }
    static QString combine(const QString& p1, const QString& p2, const QString& p3, const QString& p4, const QString& p5) { return QDir::cleanPath(p1 + '/' + p2 + '/' + p3 + '/' + p4 + '/' + p5); }
    static QString combine(const QStringList& parts);
};

#endif // PATHUTIL_H
