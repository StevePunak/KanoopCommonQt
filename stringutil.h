#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <QString>
#include "kanoopcommon.h"

class KANOOP_EXPORT StringUtil
{
public:
    static bool toBool(const QString& value) { return value.toLower() == "true" || value == "1"; }
    static bool toBool(int value) { return value != 0; }
    static QString toString(bool value) { return value ? "true" : "false"; }
    static QString toDelimitedString(const QStringList& list, char delimiter = ' ');
};

#endif // STRINGUTIL_H
