/******************************************************************************************
**
** logcategoryprivate.h
**
** Moved from my Tesseract Engineering repo to open-source
**
** Author:  Stephen Punak
** Created: Wed Oct 4 13:25:38 2023
**
******************************************************************************************/
#ifndef LOGCATEGORYPRIVATE_H
#define LOGCATEGORYPRIVATE_H
#include "logcategory.h"

#include <QMap>

namespace Log
{

class LogCategoryPrivate : public LogCategory
{
public:
    static LogCategoryPrivate* create(const QString& name, LogLevel level = DefaultLogLevel);

    void setLevel(LogLevel value) { _level = value; }

    bool isChildOf(const LogCategory& other) const;

private:
    LogCategoryPrivate(const QString& name, LogLevel level) :
        LogCategory(name)
    {
        setLevel(level);
    }
};

} // namespace Log

#endif // LOGCATEGORYPRIVATE_H
