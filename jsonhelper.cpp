#include "jsonhelper.h"


bool JsonHelper::tryGetString(const QJsonObject &object, const QString &key, QString &value)
{
    QJsonObject::const_iterator iter = object.constFind(key);
    if(iter != object.end() && iter.value().isString())
    {
        value = object[key].toString();
        return true;
    }
    return false;
}

bool JsonHelper::tryGetObject(const QJsonObject &object, const QString &key, QJsonObject &value)
{
    QJsonObject::const_iterator iter = object.constFind(key);
    if(iter != object.end() && iter.value().isObject())
    {
        value = object[key].toObject();
        return true;
    }
    return false;
}

bool JsonHelper::tryGetArray(const QJsonObject &object, const QString &key, QJsonArray &value)
{
    QJsonObject::const_iterator iter = object.constFind(key);
    if(iter != object.end() && iter.value().isArray())
    {
        value = object[key].toArray();
        return true;
    }
    return false;
}

bool JsonHelper::tryGetInt32(const QJsonObject &object, const QString &key, qint32 &value)
{
    QJsonObject::const_iterator iter = object.constFind(key);
    if(iter != object.end() && iter.value().isDouble())
    {
        value = object[key].toInt();
        return true;
    }
    return false;
}

bool JsonHelper::tryGetUInt32(const QJsonObject &object, const QString &key, quint32 &value)
{
    QJsonObject::const_iterator iter = object.constFind(key);
    if(iter != object.end() && iter.value().isDouble())
    {
        value = object[key].toInt();
        return true;
    }
    return false;
}

bool JsonHelper::tryGetDouble(const QJsonObject &object, const QString &key, qreal &value)
{
    QJsonObject::const_iterator iter = object.constFind(key);
    if(iter != object.end() && iter.value().isDouble())
    {
        value = object[key].toDouble();
        return true;
    }
    return false;
}

bool JsonHelper::tryGetBool(const QJsonObject &object, const QString &key, bool &value)
{
    QJsonObject::const_iterator iter = object.constFind(key);
    if(iter != object.end() && iter.value().isBool())
    {
        value = object[key].toBool();
        return true;
    }
    return false;
}
