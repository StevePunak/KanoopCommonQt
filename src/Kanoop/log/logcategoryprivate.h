/******************************************************************************************
**
** logcategoryprivate.h
**
** Copyright (C) Tesseract Engineering, Inc - All Rights Reserved
**
** This source code is protected under international copyright law.  All rights
** reserved and protected by the copyright holders.
**
** This file is confidential and only available to authorized individuals with the
** permission of the copyright holders.  If you encounter this file and do not have
** permission, please contact the copyright holders and delete this file.
**
** Author:  Stephen Punak
** Created: Wed Oct 4 13:25:38 2023
**
******************************************************************************************/
#ifndef LOGCATEGORYPRIVATE_H
#define LOGCATEGORYPRIVATE_H
#include "logcategory.h"

#include <QMap>

#include <Kanoop/cryptoutil.h>

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
