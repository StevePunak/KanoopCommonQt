#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <QString>
#include <QStringList>
#include "kanoopcommon.h"

class KANOOP_EXPORT StringUtil
{
public:
    /**
     * Convert the give string to a boolean:
     *   1 or 'true'   = true
     *   other         = false
     */
    static bool toBool(const QString& value) { return value.toLower() == "true" || value == "1"; }
    /**
     * Convert the integer to a boolean
     */
    static bool toBool(int value) { return value != 0; }
    /**
     * Convert a boolean to a string (true or false)
     */
    static QString toString(bool value) { return value ? "true" : "false"; }
    /**
     * Convert a list of strings to a single string delimited by the supplied character
     */
    static QString toDelimitedString(const QStringList& list, char delimiter = ' ');
    /**
     * Format value as abbreviated byte count (K = kilobytes, M = megabytes, G=gigabytes)
     */
    static QString toKMG(qint64 byteCount);
};

#endif // STRINGUTIL_H
