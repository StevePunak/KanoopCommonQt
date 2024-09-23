#ifndef IDESERIALIZABLEFROMJSON_H
#define IDESERIALIZABLEFROMJSON_H

#include <QJsonArray>
#include <QJsonObject>

class IDeserializableFromJson
{
public:
    virtual void deserializeFromJson(const QByteArray& json) = 0;
};

class IDeserializableFromJsonObject
{
public:
    virtual void deserializeFromJsonObject(const QJsonObject& jsonObject) = 0;
};

class IDeserializableFromJsonArray
{
public:
    virtual void deserializeFromJsonArray(const QJsonArray& jsonArray) = 0;
};

#endif // IDESERIALIZABLEFROMJSON_H
