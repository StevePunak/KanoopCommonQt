#ifndef IDESERIALIZABLEFROMXML_H
#define IDESERIALIZABLEFROMXML_H

#include <QByteArray>
#include <QDomDocument>
#include <QList>

class IDeserializableFromXml
{
public:
    virtual void deserializeFromXml(const QByteArray& xml) = 0;
};

class IDeserializableFromDomElement
{
public:
    virtual void deserializeFromDomElement(const QDomElement& element) = 0;
};

class IDeserializableFromDomNodeList
{
public:
    virtual void deserializeFromDomElementList(const QList<QDomElement>& list) = 0;
};

#endif // IDESERIALIZABLEFROMXML_H
