/******************************************************************************************
**
** loggingtypes.h
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
** Created: Tue Oct 3 09:44:40 2023
**
******************************************************************************************/
#ifndef LOGGINGTYPES_H
#define LOGGINGTYPES_H
#include <syslog.h>

namespace Log {

enum LogLevel
{
    Emergency       = LOG_EMERG,        // 0
    Alert           = LOG_ALERT,        // 1
    Critical        = LOG_CRIT,         // 2
    Error           = LOG_ERR,          // 3
    Warning         = LOG_WARNING,      // 4
    Notice          = LOG_NOTICE,       // 5
    Info            = LOG_INFO,         // 6
    Debug           = LOG_DEBUG,        // 7
};

enum OutputFlags
{
    None                        = 0x00000000,
    LineNumbers                 = 0x00000001,
    Timestamp                   = 0x00000002,
    Level                       = 0x00000004,
    Console                     = 0x00001000,
    File                        = 0x00002000,
    QDebug                      = 0x00004000,
    Syslog                      = 0x00008000,

    Standard = LineNumbers | Timestamp | Level | Console
};

} // namespace Log

#endif // LOGGINGTYPES_H
