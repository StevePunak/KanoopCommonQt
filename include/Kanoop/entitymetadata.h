#ifndef ENTITYMETADATA_H
#define ENTITYMETADATA_H

#include <QVariant>
#include <Kanoop/kanoopcommon.h>

class EntityMetadataInfo;

class EntityMetadata
{
public:
    EntityMetadata() :
        _type(0) {}

    EntityMetadata(int type) :
        _type(type) {}

    EntityMetadata(int type, const QVariant& data, KANOOP::ModelRole role = KANOOP::DataRole) :
        _type(type)
    {
        _data[role] = data;
    }

    int type() const { return _type; }
    QString typeString() const;

    QVariant data(KANOOP::ModelRole role = KANOOP::DataRole) const { return _data.value(role); }
    void setData(const QVariant& value, KANOOP::ModelRole role = KANOOP::DataRole) { _data.insert(role, value); }
    bool hasData(KANOOP::ModelRole role) const { return _data.contains(role); }

    QVariant toVariant() const { return  QVariant::fromValue<EntityMetadata>(*this); }
    static EntityMetadata fromVariant(const QVariant& value) { return value.value<EntityMetadata>(); }

    bool isValid() const { return _type != 0; }

    static void registerMetadata(int type, const QString& name);

    static QString getTypeString(int type);
    static int getTypeFromString(const QString& value);

private:
    int _type;
    QMap<KANOOP::ModelRole, QVariant> _data;

    static QMap<int, EntityMetadataInfo> _registeredTypes;
};

class EntityMetadataList : public QList<EntityMetadata>
{
public:
    QVariant toVariant() const { return QVariant::fromValue<EntityMetadataList>(*this); }
    static EntityMetadataList fromVariant(const QVariant& value) { return value.value<EntityMetadataList>(); }
};

Q_DECLARE_METATYPE(EntityMetadata)
Q_DECLARE_METATYPE(EntityMetadataList)

#endif // ENTITYMETADATA_H
