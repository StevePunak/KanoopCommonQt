#ifndef ISERIALIZABLETOJSON_H
#define ISERIALIZABLETOJSON_H

#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>

class ISerializableToJson
{
public:
    virtual QByteArray serializeToJson() const = 0;
};

class ISerializableToJsonObject
{
public:
    virtual QJsonObject serializeToJsonObject() const = 0;
};

class ISerializableIntoJsonObject
{
public:
    virtual void serializeIntoJsonObject(QJsonObject& object) const = 0;
};

class ISerializableToJsonArray
{
public:
    virtual QJsonArray serializeToJsonArray() const = 0;
};

#endif // ISERIALIZABLETOJSON_H
