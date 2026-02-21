/**
 * @brief Interfaces for serializing objects to XML.
 */
#ifndef ISERIALIZABLETOXML_H
#define ISERIALIZABLETOXML_H
#include <QDomDocument>
#include <QList>

/**
 * @brief Interface for objects that can serialize themselves to a raw XML byte array.
 */
class ISerializableToXml
{
public:
    /**
     * @brief Serialize this object to an XML byte array.
     * @return UTF-8 encoded XML representation of this object
     */
    virtual QByteArray serializeToXml() const = 0;
};

/**
 * @brief Interface for objects that can serialize themselves to a QDomElement.
 */
class ISerializableToDomElement
{
public:
    /**
     * @brief Serialize this object into a QDomElement.
     * @return DOM element representing this object
     */
    virtual QDomElement serializeToDomElement() const = 0;
};

/**
 * @brief Interface for objects that can serialize themselves to a list of QDomElements.
 */
class ISerializableToDomElementList
{
public:
    /**
     * @brief Serialize this object into a list of QDomElements.
     * @return List of DOM elements representing this object's data
     */
    virtual QList<QDomElement> serializeToDomElementList() const = 0;
};

#endif // ISERIALIZABLETOXML_H
