/**
 *  StringUtil
 *
 *  Some static helper methods for converting strings to and from other types.
 *
 *  Additionally, methods exist for fuzzy string comparisons using the BITAP
 *  and Levenshtein Distance algorithms.
 *
 *  Stephen Punak, February 12 2021
 */
#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <QChar>
#include <QList>
#include <QString>
#include <QStringList>
#include <QUuid>
#include "kanoopcommon.h"

/**
 * @brief Static helper methods for string conversion, manipulation, and fuzzy matching.
 *
 * Provides conversions between strings and common types (bool, double, QByteArray),
 * trimming, splitting with quote awareness, and fuzzy matching via the BITAP and
 * Levenshtein Distance algorithms.
 */
class KANOOP_EXPORT StringUtil
{
public:
    /**
     * @brief Convert the given string to a boolean.
     *
     * "true" (case-insensitive) or "1" yields true; everything else yields false.
     * @param value String to convert
     * @return Resulting boolean
     */
    static bool toBool(const QString& value) { return value.toLower() == "true" || value == "1"; }
    /**
     * @brief Convert an integer to a boolean.
     * @param value Integer to convert (0 = false, non-zero = true)
     * @return Resulting boolean
     */
    static bool toBool(int value) { return value != 0; }
    /**
     * @brief Convert a string to a byte array.
     * @param value String to convert
     * @return Resulting QByteArray
     */
    static QByteArray toByteArray(const QString& value);
    /**
     * @brief Convert a boolean to a string ("true" or "false").
     * @param value Boolean to convert
     * @return "true" or "false"
     */
    static QString toString(bool value) { return value ? "true" : "false"; }
    /**
     * @brief Convert a double to a string, trimming trailing zeros.
     * @param value Double to convert
     * @param precision Maximum number of decimal places
     * @param forcePadding When true, retain trailing zeros to the full precision
     * @return Formatted string
     */
    static QString toString(double value, int precision = 6, bool forcePadding = false);
    /**
     * @brief Convert a byte array to a hex string.
     * @param value Byte array to convert
     * @param delimiter Separator between hex bytes
     * @return Hex string representation
     */
    static QString toString(const QByteArray& value, const QString& delimiter = QString(" "));
    /**
     * @brief Convert a byte array to a hexadecimal table.
     * @param buffer Byte array to display
     * @param showOffset Whether to show byte offsets
     * @param showText Whether to show ASCII text column
     * @param tableWidth Number of bytes per row
     * @return Formatted hex table string
     */
    static QString toHexTable(const QByteArray& buffer, bool showOffset = true, bool showText = true, int tableWidth = 16);
    /**
     * @brief Convert a list of QUuids to a string.
     * @param value List of UUIDs
     * @param delimiter Separator between UUIDs
     * @return Delimited string of UUIDs
     */
    static QString toString(const QList<QUuid>& value, const QString& delimiter = QString(" "));
    /**
     * @brief Convert a delimited string to a list of UUIDs.
     * @param value Delimited string containing UUIDs
     * @param delimiter Separator between UUIDs
     * @return List of parsed QUuids
     */
    static QList<QUuid> uuidsFromString(const QString& value, const QString& delimiter = QString(" "));
    /**
     * @brief Convert a list of strings to a single delimited string.
     * @param list String list to join
     * @param delimiter Separator character
     * @return Joined string
     */
    static QString toDelimitedString(const QStringList& list, char delimiter = ' ');
    /**
     * @brief Convert a list of ints to a single delimited string.
     * @param list Integer list to join
     * @param delimiter Separator character
     * @return Joined string
     */
    static QString toDelimitedString(const QList<int>& list, char delimiter = ' ');
    /**
     * @brief Convert a list of uint8_t values to a single delimited string.
     * @param list Byte list to join
     * @param delimiter Separator character
     * @return Joined string
     */
    static QString toDelimitedString(const QList<uint8_t>& list, char delimiter = ' ');
    /**
     * @brief Format a byte count as an abbreviated string (K, M, G).
     * @param byteCount Number of bytes
     * @return Abbreviated string (e.g. "1.5 M")
     */
    static QString toKMG(qint64 byteCount);
    /**
     * @brief Trim the given characters from the front of the string.
     * @param value Source string
     * @param chars Characters to trim
     * @return Trimmed string
     */
    static QString trimFront(const QString& value, const QList<QChar> &chars);

    /**
     * @brief Trim trailing whitespace from the string.
     * @param value Source string
     * @return String with trailing whitespace removed
     */
    static QString trimEnd(const QString& value);
    /**
     * @brief Trim the given characters from the end of the string.
     * @param value Source string
     * @param chars Characters to trim
     * @return Trimmed string
     */
    static QString trimEnd(const QString& value, const QList<QChar> &chars);
    /**
     * @brief Trim the given characters from both ends of the string.
     * @param value Source string
     * @param chars Characters to trim
     * @return Trimmed string
     */
    static QString trimBothEnds(const QString& value, const QList<QChar> &chars);

    /**
     * @brief Return the region between the outermost quotes.
     * @param value Quoted string
     * @return Unquoted content
     */
    static QString unquoted(const QString& value);
    /**
     * @brief Wrap a string in double quotes.
     * @param value String to quote
     * @return Quoted string
     */
    static QString quoted(const QString& value) { return QString("\"%1\"").arg(value); }

    /**
     * @brief Return a list of all the strings trimmed.
     * @param value String list to trim
     * @param behavior Whether to keep or skip empty parts
     * @return List of trimmed strings
     */
    static QStringList trimmed(const QStringList& value, Qt::SplitBehavior behavior = Qt::KeepEmptyParts);

    /**
     * @brief Split the string respecting quoted substrings.
     * @param value String to split
     * @param separator Separator character
     * @param behavior Whether to keep or skip empty parts
     * @return Split string list
     */
    static QStringList splitWithQuotes(const QString& value, QChar separator, Qt::SplitBehavior behavior = Qt::KeepEmptyParts);
    /**
     * @brief Split the string respecting quoted substrings, using multiple separators.
     * @param value String to split
     * @param separators List of separator characters
     * @param behavior Whether to keep or skip empty parts
     * @return Split string list
     */
    static QStringList splitWithQuotes(const QString& value, QList<QChar> separators, Qt::SplitBehavior behavior = Qt::KeepEmptyParts);

    /**
     * @brief Combine strings from the list until hitting one that ends with an EOL character.
     * @param lines Source string list
     * @param index Starting index
     * @param eolCharacter End-of-line character to look for
     * @param consumed Output set to the number of lines consumed (may be nullptr)
     * @return Combined string
     */
    static QString combineToEol(const QStringList& lines, int index, const QChar& eolCharacter = ';', int* consumed = nullptr);

    /**
     * @brief Return the character index of a given word number (1-based) in the string.
     * @param value Source string
     * @param wordNumber 1-based word ordinal
     * @return Character index of the word, or -1 if not found
     */
    static int indexOfWord(const QString &value, int wordNumber);

    /**
     * @brief Calculate the index of a fuzzy match using the BITAP algorithm.
     * @param needle Pattern to search for
     * @param haystack String to search in
     * @param maxDistance Maximum Levenshtein distance for a match
     * @return Index of the match, or -1 if not found
     */
    static int fuzzyIndexOf(const QString& needle, const QString& haystack, int maxDistance = 1);

    /**
     * @brief Calculate the Levenshtein distance between two strings.
     * @param s1 First string
     * @param s2 Second string
     * @return Edit distance between the strings
     */
    static int levenshteinDistance(const QString& s1, const QString& s2);

    /**
     * @brief Implements BITAP algorithm for fuzzy search.
     */
    class Bitap
    {
    public:
        /**
         * @brief Construct a Bitap fuzzy search and compute the result.
         * @param haystack String to search in
         * @param needle Pattern to search for
         * @param maxDistance Maximum allowed Levenshtein distance
         */
        Bitap(const QString& haystack, const QString& needle, int maxDistance = 1);

        /**
         * @brief Return the index of the fuzzy match.
         * @return Match index, or -1 if no match was found
         */
        int index() const { return _index; }

    private:
        int calculate(int maxDistance);

        QString _haystack;
        QString _needle;

        int _index;
    };

    /**
     * @brief Implements the Levenshtein algorithm to determine the distance
     * between two strings.
     */
    class Levenshtein
    {
    public:
        /**
         * @brief Construct a Levenshtein calculator and compute the distance.
         * @param s1 First string
         * @param s2 Second string
         */
        Levenshtein(const QString& s1, const QString& s2);

        /**
         * @brief Return the computed Levenshtein distance.
         * @return Edit distance
         */
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
