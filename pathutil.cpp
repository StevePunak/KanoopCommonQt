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
