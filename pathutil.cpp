#include "pathutil.h"
#include <QDir>
#include <QDebug>
#include <QProcessEnvironment>

QString PathUtil::fileName(const QString &path)
{
    QFileInfo info(path);
    return info.fileName ();
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

QString PathUtil::extension(const QString &path)
{
    QString ret;
    int index = path.lastIndexOf('.');
    if(index < 0)
        ret = path;
    else
        ret = path.mid(index);

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

/**
 * @brief Find first executable in path matching basename
 * @param executable file basename
 * @return
 */
QFileInfo PathUtil::which(const QString &basename)
{
    QFileInfo result;
    QString fullPath = QProcessEnvironment::systemEnvironment().value("PATH");
    QStringList paths = fullPath.split(':');
    foreach(const QString& path, paths)
    {
        QDir dir(path);
        if(dir.exists() == false)
            continue;
        QFileInfo testFile(combine(path, basename));
        if(testFile.exists() && testFile.isExecutable())
        {
            result = testFile;
            break;
        }
    }
    return result;
}

QString PathUtil::popLevel(const QString &path)
{
    QString result;
    int index;
    if((index = path.lastIndexOf('/')) > 0)
        result = path.mid(0, index);
    return result;
}
