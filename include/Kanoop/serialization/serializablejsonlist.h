/**
 * @brief Template helpers for serializing and deserializing lists of JSON-capable objects.
 */
#ifndef SERIALIZABLEJSONLIST_H
#define SERIALIZABLEJSONLIST_H
#include <QJsonArray>
#include <QList>
#include "iserializabletojson.h"
#include "ideserializablefromjson.h"

/**
 * @brief A QList subclass that serializes/deserializes its elements via ISerializableToJsonObject.
 *
 * @tparam T Element type; must derive from both ISerializableToJsonObject and
 *           IDeserializableFromJsonObject.
 */
template <typename T>
class SerializableJsonList : public QList<T>
{
public:
    /**
     * @brief Serialize all elements to a QJsonArray.
     *
     * Each element's serializeToJsonObject() result is appended as a JSON object.
     * @return QJsonArray containing one entry per element
     */
    QJsonArray serializeToJsonArray() const {
        QJsonArray jsonArray;
        static_assert(std::is_base_of<ISerializableToJsonObject, T>::value, "T not derived from ISerializableToJsonObject");
        for(typename QList<T>::const_iterator it = this->constBegin();it != this->constEnd();it++) {
            T item = *it;
            ISerializableToJsonObject* p = static_cast<ISerializableToJsonObject*>(&item);
            QJsonObject obj = p->serializeToJsonObject();
            jsonArray.append(obj);
        }
        return jsonArray;
    }

    /**
     * @brief Populate this list by deserializing each element of a QJsonArray.
     *
     * Each array entry is passed to a default-constructed element's
     * deserializeFromJsonObject() method.
     * @param jsonArray Source JSON array
     */
    void deserializeFromJsonArray(const QJsonArray& jsonArray) {
        static_assert(std::is_base_of<IDeserializableFromJsonObject, T>::value, "T not derived from IDeserializableFromJsonObject");
        for(const QJsonValue& jsonValue : jsonArray) {
            T item;
            IDeserializableFromJsonObject* ptr = static_cast<IDeserializableFromJsonObject*>(&item);
            ptr->deserializeFromJsonObject(jsonValue.toObject());
            this->append(item);
        }
    }
};

/**
 * @brief A QList of QSharedPointer<T> that serializes/deserializes its elements via ISerializableToJsonObject.
 *
 * @tparam T Element type; must derive from both ISerializableToJsonObject and
 *           IDeserializableFromJsonObject.
 */
template <typename T>
class SerializableJsonPtrList : public QList<QSharedPointer<T>>
{
public:
    /**
     * @brief Serialize all elements to a QJsonArray.
     * @return QJsonArray containing one entry per element
     */
    QJsonArray serializeToJsonArray() const {
        QJsonArray jsonArray;
        static_assert(std::is_base_of<ISerializableToJsonObject, T>::value, "T not derived from ISerializableToJsonObject");
        for(typename QList<QSharedPointer<T>>::const_iterator it = this->constBegin();it != this->constEnd();it++) {
            QSharedPointer<T> item = *it;
            QSharedPointer<ISerializableToJsonObject> ptr = qSharedPointerCast<ISerializableToJsonObject>(item);
            QJsonObject obj = ptr->serializeToJsonObject();
            jsonArray.append(obj);
        }
        return jsonArray;
    }

    /**
     * @brief Populate this list by deserializing each element of a QJsonArray.
     * @param jsonArray Source JSON array
     */
    void deserializeFromJsonArray(const QJsonArray& jsonArray) {
        static_assert(std::is_base_of<IDeserializableFromJsonObject, T>::value, "T not derived from IDeserializableFromJsonObject");
        for(const QJsonValue& jsonValue : jsonArray) {
            QSharedPointer<T> item(new T());
            QSharedPointer<IDeserializableFromJsonObject> ptr = qSharedPointerCast<IDeserializableFromJsonObject>(item);
            ptr->deserializeFromJsonObject(jsonValue.toObject());
            this->append(item);
        }
    }
};

#endif // SERIALIZABLEJSONLIST_H
