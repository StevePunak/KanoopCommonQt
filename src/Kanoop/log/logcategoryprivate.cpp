/******************************************************************************************
**
** logcategoryprivate.cpp
**
**
** Author:  Stephen Punak
** Created: Wed Oct 4 13:25:38 2023
**
******************************************************************************************/
#include "logcategoryprivate.h"

using namespace Log;


LogCategoryPrivate *LogCategoryPrivate::create(const QString &name, LogLevel level)
{
    LogCategoryPrivate* result = new LogCategoryPrivate(name, level);
    return result;
}

bool LogCategoryPrivate::isChildOf(const LogCategory &other) const
{
    QString needle = other.name() + '.';
    return _name.startsWith(needle);
}
