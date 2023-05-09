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
     * Convert the string to a byte array
     */
    static QByteArray toByteArray(const QString& value);
    /**
     * Convert a boolean to a string (true or false)
     */
    static QString toString(bool value) { return value ? "true" : "false"; }
    /**
     * Convert a double to string, trimming trailing zeros
     */
    static QString toString(double value, int precision = 6);
    /**
     * Convert a byte array to a hex string
     */
    static QString toString(const QByteArray& value, const QString& delimiter = QString(" "));
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

    /**
     * @brief fuzzyIndexOf
     * Calculate the index of a fuzzy match of 'needle' into 'haystack' using BITAP algorithm.
     * Distance is the max Levenshtein distance to obtain a match.
     * @param needle
     * @param haystack
     * @param maxDistance
     * @return
     */
    static int fuzzyIndexOf(const QString& needle, const QString& haystack, int maxDistance = 1);

    /**
     * @brief levenshteinDistance
     * Calculate the Levenshtein distance between two strings
     * @param s1
     * @param s2
     * @return
     */
    static int levenshteinDistance(const QString& s1, const QString& s2);

    /**
     * @brief Implements BITAP algorithm for fuzzy search
     */
    class Bitap
    {
    public:
        Bitap(const QString& haystack, const QString& needle, int maxDistance = 1);

        int index() const { return _index; }

    private:
        int calculate(int maxDistance);

        QString _haystack;
        QString _needle;

        int _index;
    };

    /**
     * @brief Implements the Levenshtein algorithm to determine the distance
     * between two strings
     */
    class Levenshtein
    {
    public:
        Levenshtein(const QString& s1, const QString& s2);

        int distance() const { return _distance; }

    private:
        int min(int a, int b, int c)
        {
            int result = -1;
            if(a <= b && a <= c) {
                result = a;
            }
            else if(b <= a && b <= c) {
                result = b;
            }
            else if(c <= a && c <= b) {
                result = c;
            }
            return result;
        }
        int _distance;
    };
};

#endif // STRINGUTIL_H
