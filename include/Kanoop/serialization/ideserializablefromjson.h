/**
 * @brief Interfaces for deserializing objects from JSON.
 */
#ifndef IDESERIALIZABLEFROMJSON_H
#define IDESERIALIZABLEFROMJSON_H

#include <QJsonArray>
#include <QJsonObject>

/**
 * @brief Interface for objects that can deserialize themselves from a raw JSON byte array.
 */
class IDeserializableFromJson
{
public:
    /**
     * @brief Populate this object from a JSON byte array.
     * @param json Raw JSON data
     */
    virtual void deserializeFromJson(const QByteArray& json) = 0;
};

/**
 * @brief Interface for objects that can deserialize themselves from a QJsonObject.
 */
class IDeserializableFromJsonObject
{
public:
    /**
     * @brief Populate this object from a QJsonObject.
     * @param jsonObject Parsed JSON object containing the data
     */
    virtual void deserializeFromJsonObject(const QJsonObject& jsonObject) = 0;
};

/**
 * @brief Interface for objects that can deserialize themselves from a QJsonArray.
 */
class IDeserializableFromJsonArray
{
public:
    /**
     * @brief Populate this object from a QJsonArray.
     * @param jsonArray Parsed JSON array containing the data
     */
    virtual void deserializeFromJsonArray(const QJsonArray& jsonArray) = 0;
};

#endif // IDESERIALIZABLEFROMJSON_H
