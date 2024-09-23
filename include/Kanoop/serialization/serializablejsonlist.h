#ifndef SERIALIZABLEJSONLIST_H
#define SERIALIZABLEJSONLIST_H
#include <QJsonArray>
#include <QList>
#include "iserializabletojson.h"
#include "ideserializablefromjson.h"

template <typename T>
class SerializableJsonList : public QList<T>
{
public:
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

#endif // SERIALIZABLEJSONLIST_H
