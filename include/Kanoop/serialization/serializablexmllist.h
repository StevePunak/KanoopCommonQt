/**
 * @brief Template helper for serializing and deserializing lists of XML-capable objects.
 */
#ifndef SERIALIZABLEXMLLIST_H
#define SERIALIZABLEXMLLIST_H

#include <QDomDocument>
#include <QList>
#include "iserializabletoxml.h"
#include "ideserializablefromxml.h"

/**
 * @brief A QList subclass that serializes/deserializes its elements via ISerializableToDomElement.
 *
 * @tparam T Element type; must derive from both ISerializableToDomElement and
 *           IDeserializableFromDomElement.
 */
template <typename T>
class SerializableXmlList : public QList<T>
{
public:
    /** @brief Equality comparison — lists are equal if they have the same elements in the same order. */
    bool operator==(const SerializableXmlList<T>& other) const
    {
        bool result = SerializableXmlList<T>::count() == other.count();
        for(int i = 0;i < SerializableXmlList<T>::count() && result == true;i++) {
            result = this->at(i) == other.at(i);
        }
        return result;
    }
    /** @brief Inequality comparison. */
    bool operator!=(const SerializableXmlList<T>& other) const { return !(*this == other); }

    /**
     * @brief Serialize all elements into a QDomElement container.
     *
     * Each element's serializeToDomElement() result is appended as a child of a
     * new element named tagName.
     * @param tagName XML tag name for the wrapper element
     * @return QDomElement with one child per list element
     */
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

    /**
     * @brief Populate this list by deserializing child elements of a QDomElement.
     *
     * Each direct child element is passed to a default-constructed element's
     * deserializeFromDomElement() method.
     * @param element Parent DOM element whose children to deserialize
     */
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
