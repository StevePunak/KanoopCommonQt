#include "pathutil.h"
#include <QDir>
#include <QDebug>

QString PathUtil::fileName(const QString &path)
{
    QString ret;
    int index = path.lastIndexOf(QDir::separator());
    if(index < 0)
        ret = path;
    else
        ret = path.mid(index + 1);

    return ret;
}

QString PathUtil::dirName(const QString &path)
{
    QString ret;
    int index = path.lastIndexOf(QDir::separator());
    if(index < 0)
        ret = path;
    else
        ret = path.mid(0, index);

    return ret;

}

QString PathUtil::trimTrailingSlash(const QString &path)
{
    int i = path.length();
    for(--i;i >= 0 && (path[i].isSpace() || path[i] == '/');i--);
    QString trimmed = ++i > 0 ? path.mid(0, i) : "";
    return trimmed;
}
