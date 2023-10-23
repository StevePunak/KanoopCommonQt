#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <QChar>
#include <QList>
#include <QString>
#include <QStringList>
#include <QUuid>
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
     * Convert a list of QUuids to a string
     */
    static QString toString(const QList<QUuid>& value, const QString& delimiter = QString(" "));
    /**
     * Convert a delimited string to a list of UUIDs
     */
    static QList<QUuid> uuidsFromString(const QString& value, const QString& delimiter = QString(" "));
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
     * @brief unquoted
     * @param value
     * @return The region between the quotes
     */
    static QString unquoted(const QString& value);
    static QString quoted(const QString& value) { return QString("\"%1\"").arg(value); }

    /**
     * @brief trimmed
     * @param value
     * @param behavior
     * @return A list of all the strings trimmed
     */
    static QStringList trimmed(const QStringList& value, Qt::SplitBehavior behavior = Qt::KeepEmptyParts);

    /**
     * @brief splitWithQuotes
     * Split the string respecting quoted strings within
     * @param value
     * @param separator
     * @param behavior
     * @return
     */
    static QStringList splitWithQuotes(const QString& value, QChar separator, Qt::SplitBehavior behavior = Qt::KeepEmptyParts);
    static QStringList splitWithQuotes(const QString& value, QList<QChar> separators, Qt::SplitBehavior behavior = Qt::KeepEmptyParts);

    /**
     * @brief combineToEol
     * Combine strings from the list until hitting the string with an EOL at the end
     * @param lines
     * @param index
     * @param eolCharacter
     * @return
     */
    static QString combineToEol(const QStringList& lines, int index, const QChar& eolCharacter = ';', int* consumed = nullptr);

    /**
     * @brief indexOfWord
     * @param wordNumber
     * @return the index of the give word number (1, 2, 3 etc...)
     */
    static int indexOfWord(const QString &value, int wordNumber);

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

private:
    class StringSplitter
    {
    public:
        static QStringList splitWithQuotes(const QString& value, QChar separator, Qt::SplitBehavior behavior = Qt::KeepEmptyParts);
        static QStringList splitWithQuotes(const QString& value, QList<QChar> separators, Qt::SplitBehavior behavior = Qt::KeepEmptyParts);

    private:
        StringSplitter(const QString& value, QList<QChar> separators, Qt::SplitBehavior behavior = Qt::KeepEmptyParts) :
            _originalString(value), _separators(separators), _behavior(behavior), _inQuote(false) {}

        void performSplit();
        void appendCurrent();

        QString _originalString;
        QList<QChar> _separators;
        Qt::SplitBehavior _behavior;

        QString _current;
        bool _inQuote;

        QStringList _result;

        static const char QUOTE = '\"';
    };

    class StringCombiner
    {
    public:
        static QString combineToEol(const QStringList& lines, int index, const QChar& eolCharacter, int* consumed = nullptr);
    };

    static const char QUOTE = '\"';
    static const char COLON = ':';
};

#endif // STRINGUTIL_H
