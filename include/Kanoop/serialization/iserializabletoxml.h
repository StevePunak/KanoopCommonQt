#ifndef ISERIALIZABLETOXML_H
#define ISERIALIZABLETOXML_H
#include <QDomDocument>
#include <QList>

class ISerializableToXml
{
public:
    virtual QByteArray serializeToXml() const = 0;
};

class ISerializableToDomElement
{
public:
    virtual QDomElement serializeToDomElement() const = 0;
};

class ISerializableToDomElementList
{
public:
    virtual QList<QDomElement> serializeToDomElementList() const = 0;
};

#endif // ISERIALIZABLETOXML_H
