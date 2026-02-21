/**
 * @brief Interfaces for deserializing objects from XML.
 */
#ifndef IDESERIALIZABLEFROMXML_H
#define IDESERIALIZABLEFROMXML_H

#include <QByteArray>
#include <QDomDocument>
#include <QList>

/**
 * @brief Interface for objects that can deserialize themselves from a raw XML byte array.
 */
class IDeserializableFromXml
{
public:
    /**
     * @brief Populate this object from an XML byte array.
     * @param xml Raw XML data
     */
    virtual void deserializeFromXml(const QByteArray& xml) = 0;
};

/**
 * @brief Interface for objects that can deserialize themselves from a QDomElement.
 */
class IDeserializableFromDomElement
{
public:
    /**
     * @brief Populate this object from a QDomElement.
     * @param element DOM element containing the object's data
     */
    virtual void deserializeFromDomElement(const QDomElement& element) = 0;
};

/**
 * @brief Interface for objects that can deserialize themselves from a list of QDomElements.
 */
class IDeserializableFromDomNodeList
{
public:
    /**
     * @brief Populate this object from a list of QDomElement values.
     * @param list List of DOM elements to deserialize from
     */
    virtual void deserializeFromDomElementList(const QList<QDomElement>& list) = 0;
};

#endif // IDESERIALIZABLEFROMXML_H
