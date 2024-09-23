#ifndef SERIALIZABLEXMLLIST_H
#define SERIALIZABLEXMLLIST_H

#include <QDomDocument>
#include <QList>
#include "iserializabletoxml.h"
#include "ideserializablefromxml.h"

template <typename T>
class SerializableXmlList : public QList<T>
{
public:
    bool operator==(const SerializableXmlList<T>& other) const
    {
        bool result = SerializableXmlList<T>::count() == other.count();
        for(int i = 0;i < SerializableXmlList<T>::count() && result == true;i++) {
            result = this->at(i) == other.at(i);
        }
        return result;
    }
    bool operator!=(const SerializableXmlList<T>& other) const { return !(*this == other); }

    QDomElement serializeToDomElement(const QString& tagName) const {
        static_assert(std::is_base_of<ISerializableToDomElement, T>::value, "T not derived from ISerializableToDomElement");
        QDomDocument doc;
        QDomElement result = doc.createElement(tagName);
        for(typename QList<T>::const_iterator it = this->constBegin();it != this->constEnd();it++) {
            T item = *it;
            ISerializableToDomElement* p = static_cast<ISerializableToDomElement*>(&item);
            QDomElement obj = p->serializeToDomElement();
            result.appendChild(obj);
        }
        return result;
    }

    void deserializeFromDomElement(const QDomElement& element) {
        static_assert(std::is_base_of<IDeserializableFromDomElement, T>::value, "T not derived from IDeserializableFromDomElement");
        for(QDomElement childElement = element.firstChildElement();childElement.isNull() == false;childElement = childElement.nextSiblingElement()) {
            T item;
            IDeserializableFromDomElement* ptr = static_cast<IDeserializableFromDomElement*>(&item);
            ptr->deserializeFromDomElement(childElement);
            this->append(item);
        }
    }
};

#endif // SERIALIZABLEXMLLIST_H
