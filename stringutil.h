#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <QChar>
#include <QList>
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
     * Convert a double to string, trimming trailing zeros
     */
    static QString toString(double value, int precision = 6);
    /**
     * Convert a list of strings to a single string delimited by the supplied character
     */
    static QString toDelimitedString(const QStringList& list, char delimiter = ' ');
    /**
     * Format value as abbreviated byte count (K = kilobytes, M = megabytes, G=gigabytes)
     */
    static QString toKMG(qint64 byteCount);
    /**
     * @brief trimFront
     * Trim the given characters from the front of the string
     */
    static QString trimFront(const QString& value, const QList<QChar> &chars);
    /**
     * @brief trimEnd
     * Trim the given characters from the end of the string
     */
    static QString trimEnd(const QString& value, const QList<QChar> &chars);
    /**
     * @brief trimBothEnds
     * Trim the given characters from both ends of the string
     */
    static QString trimBothEnds(const QString& value, const QList<QChar> &chars);
};

#endif // STRINGUTIL_H
