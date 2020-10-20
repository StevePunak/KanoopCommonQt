#ifndef PATHUTIL_H
#define PATHUTIL_H
#include <QString>

class PathUtil
{
public:
    static QString fileName(const QString& path);
    static QString dirName(const QString& path);
    static QString trimTrailingSlash(const QString& path);
};

#endif // PATHUTIL_H
