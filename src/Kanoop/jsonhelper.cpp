#include "Kanoop/jsonhelper.h"

#include <QJsonDocument>
#include <stringutil.h>


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

QString JsonHelper::toIndented(const QByteArray &json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QString output = doc.toJson(QJsonDocument::Indented);
    return output;
}

QJsonArray JsonHelper::toJsonArray(const QStringList &value)
{
    QJsonArray result;
    for(const QString& s : value) {
        result.append(s);
    }
    return result;
}

QStringList JsonHelper::toStringList(const QJsonArray &value)
{
    QStringList result;
    for(const QJsonValue& v : value) {
        result.append(v.toString());
    }
    return result;
}

void JsonHelper::appendToArray(QJsonArray& destArray, const QJsonArray& sourceArray)
{
    for(const QJsonValue& value : sourceArray) {
        destArray.append(value);
    }
}

QJsonValue JsonHelper::doubleStringOrNull(const QVariant& value, int precision)
{
    return value.isNull() ? QJsonValue() : QString("%1").arg(value.toDouble(), 0, 'f', precision);
}

QJsonValue JsonHelper::arrayOrNull(const QStringList& value)
{
    if(value.count() == 0) { return QJsonValue(); }
    QJsonArray result;
    for(const QString& text : value) { result.append(text); }
    return result;
}

QVariant JsonHelper::doubleOrNull(const QJsonValue& value)
{
    QVariant result;
    if(value.isString()) return value.toString().isEmpty() ? result : value.toString().toDouble();
    return value.isNull() ? result : value.toDouble();
}

QVariant JsonHelper::doubleStringOrNull(const QJsonValue& value, int precision)
{
    QVariant result;
    return value.isNull() ? result : StringUtil::toString(value.toString().toDouble(), precision);
}

