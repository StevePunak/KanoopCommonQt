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

protected:
    static QJsonValue uuidOrNull(const QUuid& uuid);
    static QJsonValue stringOrNull(const QString& value);
    static QJsonValue intOrNull(const QVariant& value);
    static QJsonValue doubleOrNull(const QVariant& value);
    static QJsonValue doubleStringOrNull(const QVariant& value, int precision = 6);
    static QJsonValue arrayOrNull(const QStringList& value);
    static QVariant intOrNull(const QJsonValue& value);
    static QVariant doubleOrNull(const QJsonValue& value);
    static QVariant doubleStringOrNull(const QJsonValue& value, int precision = 6);
};

class ISerializableIntoJsonObject
{
public:
    virtual void serializeIntoJsonObject(QJsonObject& object) const = 0;

protected:
    static QJsonValue uuidOrNull(const QUuid& uuid);
    static QJsonValue stringOrNull(const QString& value);
    static QJsonValue intOrNull(const QVariant& value);
    static QJsonValue doubleOrNull(const QVariant& value);
    static QJsonValue doubleStringOrNull(const QVariant& value, int precision = 6);
    static QJsonValue arrayOrNull(const QStringList& value);
    static QVariant intOrNull(const QJsonValue& value);
    static QVariant doubleOrNull(const QJsonValue& value);
    static QVariant doubleStringOrNull(const QJsonValue& value, int precision = 6);
};

class ISerializableToJsonArray
{
public:
    virtual QJsonArray serializeToJsonArray() const = 0;
};

#endif // ISERIALIZABLETOJSON_H
