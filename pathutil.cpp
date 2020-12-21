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

QString PathUtil::combine(const QStringList &parts)
{
    QString result;
    result.reserve(1024);
    QTextStream output(&result);
    for(int i = 0;i < parts.length();i++)
    {
        output << parts[i];
        if(i < parts.length() - 1)
            output << '/';
    }
    return result;
}
