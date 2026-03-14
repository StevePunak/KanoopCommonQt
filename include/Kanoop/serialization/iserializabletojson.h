/**
 * @brief Interfaces for serializing objects to JSON.
 */
#ifndef ISERIALIZABLETOJSON_H
#define ISERIALIZABLETOJSON_H

#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>
#include <Kanoop/kanoopcommon.h>

/**
 * @brief Interface for objects that can serialize themselves to a raw JSON byte array.
 */
class KANOOP_EXPORT ISerializableToJson
{
public:
    /**
     * @brief Serialize this object to a JSON byte array.
     * @return UTF-8 encoded JSON representation
     */
    virtual QByteArray serializeToJson() const = 0;
};

/**
 * @brief Interface for objects that can serialize themselves into a QJsonObject, with JSON helper utilities.
 */
class KANOOP_EXPORT ISerializableToJsonObject
{
public:
    /**
     * @brief Serialize this object into a QJsonObject.
     * @return JSON object representation of this object
     */
    virtual QJsonObject serializeToJsonObject() const = 0;

protected:
    /**
     * @brief Return a UUID as a JSON string, or null if uuid is null.
     * @param uuid UUID to convert
     * @return JSON string value, or QJsonValue::Null
     */
    static QJsonValue uuidOrNull(const QUuid& uuid);
    /**
     * @brief Return a string as a JSON string, or null if value is empty.
     * @param value String to convert
     * @return JSON string value, or QJsonValue::Null
     */
    static QJsonValue stringOrNull(const QString& value);
    /**
     * @brief Return a QVariant integer as a JSON integer, or null if variant is null.
     * @param value Variant containing an integer
     * @return JSON integer value, or QJsonValue::Null
     */
    static QJsonValue intOrNull(const QVariant& value);
    /**
     * @brief Return a QVariant double as a JSON double, or null if variant is null.
     * @param value Variant containing a double
     * @return JSON double value, or QJsonValue::Null
     */
    static QJsonValue doubleOrNull(const QVariant& value);
    /**
     * @brief Return a QVariant double formatted as a JSON string, or null if variant is null.
     * @param value Variant containing a double
     * @param precision Number of decimal places (default 6)
     * @return JSON string value, or QJsonValue::Null
     */
    static QJsonValue doubleStringOrNull(const QVariant& value, int precision = 6);
    /**
     * @brief Return a string list as a JSON array, or null if list is empty.
     * @param value String list to convert
     * @return JSON array value, or QJsonValue::Null
     */
    static QJsonValue arrayOrNull(const QStringList& value);
    /**
     * @brief Extract an integer QVariant from a JSON value, or invalid QVariant if null.
     * @param value JSON value to extract from
     * @return QVariant containing an integer, or an invalid QVariant
     */
    static QVariant intOrNull(const QJsonValue& value);
    /**
     * @brief Extract a double QVariant from a JSON value, or invalid QVariant if null.
     * @param value JSON value to extract from
     * @return QVariant containing a double, or an invalid QVariant
     */
    static QVariant doubleOrNull(const QJsonValue& value);
    /**
     * @brief Parse a double from a JSON string value, or invalid QVariant if null.
     * @param value JSON value to parse
     * @param precision Number of decimal places (default 6)
     * @return QVariant containing a double, or an invalid QVariant
     */
    static QVariant doubleStringOrNull(const QJsonValue& value, int precision = 6);
};

/**
 * @brief Interface for objects that can serialize into an existing QJsonObject (in-place variant).
 */
class KANOOP_EXPORT ISerializableIntoJsonObject
{
public:
    /**
     * @brief Write this object's fields into an existing QJsonObject.
     * @param object JSON object to populate
     */
    virtual void serializeIntoJsonObject(QJsonObject& object) const = 0;

protected:
    /**
     * @brief Return a UUID as a JSON string, or null if uuid is null.
     * @param uuid UUID to convert
     * @return JSON string value, or QJsonValue::Null
     */
    static QJsonValue uuidOrNull(const QUuid& uuid);
    /**
     * @brief Return a string as a JSON string, or null if value is empty.
     * @param value String to convert
     * @return JSON string value, or QJsonValue::Null
     */
    static QJsonValue stringOrNull(const QString& value);
    /**
     * @brief Return a QVariant integer as a JSON integer, or null if variant is null.
     * @param value Variant containing an integer
     * @return JSON integer value, or QJsonValue::Null
     */
    static QJsonValue intOrNull(const QVariant& value);
    /**
     * @brief Return a QVariant double as a JSON double, or null if variant is null.
     * @param value Variant containing a double
     * @return JSON double value, or QJsonValue::Null
     */
    static QJsonValue doubleOrNull(const QVariant& value);
    /**
     * @brief Return a QVariant double formatted as a JSON string, or null if variant is null.
     * @param value Variant containing a double
     * @param precision Number of decimal places (default 6)
     * @return JSON string value, or QJsonValue::Null
     */
    static QJsonValue doubleStringOrNull(const QVariant& value, int precision = 6);
    /**
     * @brief Return a string list as a JSON array, or null if list is empty.
     * @param value String list to convert
     * @return JSON array value, or QJsonValue::Null
     */
    static QJsonValue arrayOrNull(const QStringList& value);
    /**
     * @brief Extract an integer QVariant from a JSON value, or invalid QVariant if null.
     * @param value JSON value to extract from
     * @return QVariant containing an integer, or an invalid QVariant
     */
    static QVariant intOrNull(const QJsonValue& value);
    /**
     * @brief Extract a double QVariant from a JSON value, or invalid QVariant if null.
     * @param value JSON value to extract from
     * @return QVariant containing a double, or an invalid QVariant
     */
    static QVariant doubleOrNull(const QJsonValue& value);
    /**
     * @brief Parse a double from a JSON string value, or invalid QVariant if null.
     * @param value JSON value to parse
     * @param precision Number of decimal places (default 6)
     * @return QVariant containing a double, or an invalid QVariant
     */
    static QVariant doubleStringOrNull(const QJsonValue& value, int precision = 6);
};

/**
 * @brief Interface for objects that can serialize themselves to a QJsonArray.
 */
class KANOOP_EXPORT ISerializableToJsonArray
{
public:
    /**
     * @brief Serialize this object into a QJsonArray.
     * @return JSON array representation of this object
     */
    virtual QJsonArray serializeToJsonArray() const = 0;
};

#endif // ISERIALIZABLETOJSON_H
