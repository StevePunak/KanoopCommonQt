#include "serialization/iserializabletojson.h"

#include <jsonhelper.h>
#include <stringutil.h>

QJsonValue ISerializableToJsonObject::uuidOrNull(const QUuid& uuid)
{
    return JsonHelper::uuidOrNull(uuid);
}

QJsonValue ISerializableToJsonObject::stringOrNull(const QString& value)
{
    return JsonHelper::stringOrNull(value);
}

QJsonValue ISerializableToJsonObject::intOrNull(const QVariant& value)
{
    return JsonHelper::intOrNull(value);
}

QJsonValue ISerializableToJsonObject::doubleOrNull(const QVariant& value)
{
    return JsonHelper::doubleOrNull(value);
}

QJsonValue ISerializableToJsonObject::doubleStringOrNull(const QVariant& value, int precision)
{
    return JsonHelper::doubleStringOrNull(value, precision);
}

QJsonValue ISerializableToJsonObject::arrayOrNull(const QStringList& value)
{
    return JsonHelper::arrayOrNull(value);
}

QVariant ISerializableToJsonObject::intOrNull(const QJsonValue& value)
{
    return JsonHelper::intOrNull(value);
}

QVariant ISerializableToJsonObject::doubleOrNull(const QJsonValue& value)
{
    return JsonHelper::doubleOrNull(value);
}

QVariant ISerializableToJsonObject::doubleStringOrNull(const QJsonValue& value, int precision)
{
    return JsonHelper::doubleStringOrNull(value, precision);
}

QJsonValue ISerializableIntoJsonObject::uuidOrNull(const QUuid& uuid)
{
    return JsonHelper::uuidOrNull(uuid);
}

QJsonValue ISerializableIntoJsonObject::stringOrNull(const QString& value)
{
    return JsonHelper::stringOrNull(value);
}

QJsonValue ISerializableIntoJsonObject::intOrNull(const QVariant& value)
{
    return JsonHelper::intOrNull(value);
}

QJsonValue ISerializableIntoJsonObject::doubleOrNull(const QVariant& value)
{
    return JsonHelper::doubleOrNull(value);
}

QJsonValue ISerializableIntoJsonObject::doubleStringOrNull(const QVariant& value, int precision)
{
    return JsonHelper::doubleStringOrNull(value, precision);
}

QJsonValue ISerializableIntoJsonObject::arrayOrNull(const QStringList& value)
{
    return JsonHelper::arrayOrNull(value);
}

QVariant ISerializableIntoJsonObject::intOrNull(const QJsonValue& value)
{
    return JsonHelper::intOrNull(value);
}

QVariant ISerializableIntoJsonObject::doubleOrNull(const QJsonValue& value)
{
    return JsonHelper::doubleOrNull(value);
}

QVariant ISerializableIntoJsonObject::doubleStringOrNull(const QJsonValue& value, int precision)
{
    return JsonHelper::doubleStringOrNull(value, precision);
}

