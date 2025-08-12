/**
 *  AddressHelper
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

class KANOOP_EXPORT JsonHelper
{
public:
    static bool tryGetString(const QJsonObject& object, const QString& key, QString& value);
    static bool tryGetObject(const QJsonObject& object, const QString& key, QJsonObject& value);
    static bool tryGetArray(const QJsonObject& object, const QString& key, QJsonArray& value);
    static bool tryGetInt32(const QJsonObject& object, const QString& key, qint32& value);
    static bool tryGetUInt32(const QJsonObject& object, const QString& key, quint32& value);
    static bool tryGetDouble(const QJsonObject& object, const QString& key, qreal& value);
    static bool tryGetBool(const QJsonObject& object, const QString& key, bool& value);
    static QString toIndented(const QByteArray& json);
    static QJsonArray toJsonArray(const QStringList& value);
    static QStringList toStringList(const QJsonArray& value);
    static QStringList fromJsonArray(const QJsonArray& value) { return toStringList(value); }
    static void appendToArray(QJsonArray& destArray, const QJsonArray& sourceArray);

    static QJsonValue uuidOrNull(const QUuid& uuid) { return uuid.isNull() ? QJsonValue() : uuid.toString(QUuid::WithoutBraces); }
    static QJsonValue stringOrNull(const QString& value) { return value.isEmpty() ? QJsonValue() : value; }
    static QJsonValue intOrNull(const QVariant& value) { return value.isNull() ? QJsonValue() : value.toInt(); }
    static QJsonValue doubleOrNull(const QVariant& value) { return value.isNull() ? QJsonValue() : value.toDouble(); }
    static QJsonValue doubleStringOrNull(const QVariant& value, int precision = 6);
    static QJsonValue arrayOrNull(const QStringList& value);
    static QVariant intOrNull(const QJsonValue& value) {  QVariant result;  return value.isNull() ? result : value.toInt(); }
    static QVariant doubleOrNull(const QJsonValue& value);
    static QVariant doubleStringOrNull(const QJsonValue& value, int precision = 6);
};

#endif // JSONHELPER_H
