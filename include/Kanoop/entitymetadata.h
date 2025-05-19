/**
 *  EntityMetadata
 *
 *  This is a container class for trivial (copyable) objects.
 *
 *  The primary idea is the EntityMetadata containing the target object can be passed
 *  around in place of the object itself. The EntityMetadata object itself can contain
 *  whatever additional data is needed by using the data() and setData() methods.
 *
 *  User defined entity types should be defined in an enumeration and registered at
 *  initialization with registerMetadata().
 *
 *  Icons ID's can be associated with specific entity types using registerIcon()
 *  when used in conjunction with KanoopCommonGuiQt
 *
 *  It is primarily intended for use by the AbstractItemView / AbstractItemModel
 *  paradigm, but has found use in several other applications.
 *
 *  Stephen Punak, September 23 2024
 */
#ifndef ENTITYMETADATA_H
#define ENTITYMETADATA_H

#include <QUuid>
#include <QVariant>
#include <Kanoop/kanoopcommon.h>

class EntityMetadataInfo;

class KANOOP_EXPORT EntityMetadata
{
public:
    EntityMetadata() :
        _type(0), _iconId(0) {}

    EntityMetadata(int type);

    EntityMetadata(int type, const QVariant& data, KANOOP::ModelRole role = KANOOP::DataRole);
    EntityMetadata(int type, const QVariant& data, const QUuid& uuid, KANOOP::ModelRole role = KANOOP::DataRole);

    int type() const { return _type; }
    QString typeString() const;

    int iconId() const { return _iconId; }

    QVariant data(int role = KANOOP::DataRole) const { return _data.value(role); }
    void setData(const QVariant& value, int role = KANOOP::DataRole) { _data.insert(role, value); }
    bool hasData(int role) const { return _data.contains(role); }

    QUuid uuid() const { return data(KANOOP::UUidRole).toUuid(); }
    void setUuid(const QUuid& value) { setData(value, KANOOP::UUidRole); }

    QVariant toVariant() const { return  QVariant::fromValue<EntityMetadata>(*this); }
    static EntityMetadata fromVariant(const QVariant& value) { return value.value<EntityMetadata>(); }

    bool isValid() const { return _type != 0; }

    static void registerMetadata(int type, const QString& name, int iconId = 0);
    static void registerIcon(int type, int iconId);

    static QString getTypeString(int type);
    static int getTypeFromString(const QString& value);
    static QString getMimeTypeString(int type) { return QString("application-epcpower/%1").arg(getTypeString(type)); }

private:
    void resolveIconId();

    int _type;
    QMap<int, QVariant> _data;
    int _iconId;

    static QMap<int, EntityMetadataInfo*> _registeredTypes;
    static QMap<QString, int> _nameToTypeMap;
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
