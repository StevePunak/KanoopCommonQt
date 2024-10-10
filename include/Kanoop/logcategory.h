/******************************************************************************************
**
** logcategory.h
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
#ifndef LOGCATEGORY_H
#define LOGCATEGORY_H

#include <QString>
#include "loggingtypes.h"
#include <Kanoop/kanoopcommon.h>

namespace Log
{

class KANOOP_EXPORT LogCategory
{
public:
    LogCategory() : _level(DefaultLogLevel) {}
    LogCategory(const QString& name) :
        _name(name),
        _level(DefaultLogLevel) {}

    QString name() const { return _name; }
    LogLevel level() const { return _level; }

    LogCategory append(const QString& name);
    LogCategory prepend(const QString& name);

    static QString parentName(const QString& name);

    static LogLevel defaultLogLevel() { return DefaultLogLevel; }
    static void setDefaultLogLevel(LogLevel value) { DefaultLogLevel = value; }

    bool isValid() const { return _name.length() != 0; }

    static LogLevel DefaultLogLevel;

protected:
    QString _name;
    LogLevel _level;
};

} // namespace Log

#endif // LOGCATEGORY_H
