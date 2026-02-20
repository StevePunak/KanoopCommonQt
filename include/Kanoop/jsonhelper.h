/**
 *  JsonHelper
 *
 *  Some static helper methods for QJson where the C# tryGet() model is
 *  desired.
 *
 *  Stephen Punak, May 16 2020
 */
#ifndef JSONHELPER_H
#define JSONHELPER_H
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include "kanoopcommon.h"

/**
 * @brief Static helper methods for reading and writing QJsonObject values safely.
 *
 * Provides tryGet-style methods that return false rather than throwing when a key
 * is absent or the wrong type, plus convenience conversions between Qt and JSON types.
 */
class KANOOP_EXPORT JsonHelper
{
public:
    /**
     * @brief Try to read a string value from a JSON object.
     * @param object Source JSON object
     * @param key Key to look up
     * @param value Output receiving the string
     * @return true if key exists and holds a string
     */
    static bool tryGetString(const QJsonObject& object, const QString& key, QString& value);

    /**
     * @brief Try to read a nested JSON object from a JSON object.
     * @param object Source JSON object
     * @param key Key to look up
     * @param value Output receiving the nested object
     * @return true if key exists and holds an object
     */
    static bool tryGetObject(const QJsonObject& object, const QString& key, QJsonObject& value);

    /**
     * @brief Try to read a JSON array from a JSON object.
     * @param object Source JSON object
     * @param key Key to look up
     * @param value Output receiving the array
     * @return true if key exists and holds an array
     */
    static bool tryGetArray(const QJsonObject& object, const QString& key, QJsonArray& value);

    /**
     * @brief Try to read a 32-bit signed integer from a JSON object.
     * @param object Source JSON object
     * @param key Key to look up
     * @param value Output receiving the integer
     * @return true if key exists and holds a numeric value
     */
    static bool tryGetInt32(const QJsonObject& object, const QString& key, qint32& value);

    /**
     * @brief Try to read a 32-bit unsigned integer from a JSON object.
     * @param object Source JSON object
     * @param key Key to look up
     * @param value Output receiving the unsigned integer
     * @return true if key exists and holds a numeric value
     */
    static bool tryGetUInt32(const QJsonObject& object, const QString& key, quint32& value);

    /**
     * @brief Try to read a double from a JSON object.
     * @param object Source JSON object
     * @param key Key to look up
     * @param value Output receiving the double
     * @return true if key exists and holds a numeric value
     */
    static bool tryGetDouble(const QJsonObject& object, const QString& key, qreal& value);

    /**
     * @brief Try to read a boolean from a JSON object.
     * @param object Source JSON object
     * @param key Key to look up
     * @param value Output receiving the boolean
     * @return true if key exists and holds a boolean
     */
    static bool tryGetBool(const QJsonObject& object, const QString& key, bool& value);

    /**
     * @brief Format a compact JSON byte array as an indented human-readable string.
     * @param json Compact JSON bytes
     * @return Indented JSON string
     */
    static QString toIndented(const QByteArray& json);

    /**
     * @brief Convert a QStringList to a QJsonArray.
     * @param value String list to convert
     * @return JSON array containing the strings
     */
    static QJsonArray toJsonArray(const QStringList& value);

    /**
     * @brief Convert a QJsonArray of strings to a QStringList.
     * @param value JSON array to convert
     * @return String list extracted from the array
     */
    static QStringList toStringList(const QJsonArray& value);

    /**
     * @brief Convert a QJsonArray of strings to a QStringList (alias for toStringList).
     * @param value JSON array to convert
     * @return String list extracted from the array
     */
    static QStringList fromJsonArray(const QJsonArray& value) { return toStringList(value); }

    /**
     * @brief Append all elements from sourceArray into destArray.
     * @param destArray Destination array to append to
     * @param sourceArray Source array whose elements are appended
     */
    static void appendToArray(QJsonArray& destArray, const QJsonArray& sourceArray);

    /**
     * @brief Return a UUID as a JSON string, or JSON null if the UUID is null.
     * @param uuid UUID to serialize
     * @return JSON string (without braces) or null value
     */
    static QJsonValue uuidOrNull(const QUuid& uuid) { return uuid.isNull() ? QJsonValue() : uuid.toString(QUuid::WithoutBraces); }

    /**
     * @brief Return a string as a JSON string, or JSON null if the string is empty.
     * @param value String to serialize
     * @return JSON string or null value
     */
    static QJsonValue stringOrNull(const QString& value) { return value.isEmpty() ? QJsonValue() : value; }

    /**
     * @brief Return a QVariant integer as a JSON integer, or JSON null if the variant is null.
     * @param value Variant holding an integer
     * @return JSON integer or null value
     */
    static QJsonValue intOrNull(const QVariant& value) { return value.isNull() ? QJsonValue() : value.toInt(); }

    /**
     * @brief Return a QVariant double as a JSON double, or JSON null if the variant is null.
     * @param value Variant holding a double
     * @return JSON double or null value
     */
    static QJsonValue doubleOrNull(const QVariant& value) { return value.isNull() ? QJsonValue() : value.toDouble(); }

    /**
     * @brief Return a QVariant double formatted as a JSON string, or JSON null if the variant is null.
     * @param value Variant holding a double
     * @param precision Number of decimal places (default 6)
     * @return JSON string representation or null value
     */
    static QJsonValue doubleStringOrNull(const QVariant& value, int precision = 6);

    /**
     * @brief Return a QStringList as a JSON array, or JSON null if the list is empty.
     * @param value String list to serialize
     * @return JSON array or null value
     */
    static QJsonValue arrayOrNull(const QStringList& value);

    /**
     * @brief Extract an integer QVariant from a JSON value, or an invalid QVariant if null.
     * @param value JSON value to read
     * @return QVariant holding the integer, or invalid QVariant
     */
    static QVariant intOrNull(const QJsonValue& value) {  QVariant result;  return value.isNull() ? result : value.toInt(); }

    /**
     * @brief Extract a double QVariant from a JSON value, or an invalid QVariant if null.
     * @param value JSON value to read
     * @return QVariant holding the double, or invalid QVariant
     */
    static QVariant doubleOrNull(const QJsonValue& value);

    /**
     * @brief Parse a double from a JSON string value, or return an invalid QVariant if null.
     * @param value JSON string value to parse
     * @param precision Precision hint (default 6)
     * @return QVariant holding the double, or invalid QVariant
     */
    static QVariant doubleStringOrNull(const QJsonValue& value, int precision = 6);
};

#endif // JSONHELPER_H
