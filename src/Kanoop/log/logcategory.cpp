/******************************************************************************************
**
** logcategory.cpp
**
**
** Author:  Stephen Punak
** Created: Wed Oct 4 13:25:38 2023
**
******************************************************************************************/
#include "logcategory.h"

using namespace Log;

LogLevel LogCategory::DefaultLogLevel       = Log::Debug;

LogCategory LogCategory::append(const QString &name)
{
    LogCategory result = LogCategory(QString("%1.%2").arg(_name).arg(name));
    result._level = _level;
    return result;
}

LogCategory LogCategory::prepend(const QString &name)
{
    LogCategory result = LogCategory(QString("%1.%2").arg(name).arg(_name));
    result._level = _level;
    return result;
}

QString LogCategory::parentName(const QString &name)
{
    QString result;
    int index = name.lastIndexOf('.');
    if(index > 0) {
        result = name.left(index);
    }
    return result;
}
