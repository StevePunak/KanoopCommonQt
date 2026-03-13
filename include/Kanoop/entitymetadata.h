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

/**
 * @brief A typed, role-keyed data container for passing objects through model/view layers.
 *
 * EntityMetadata wraps an integer entity type together with arbitrary role-keyed QVariant data
 * and an optional UUID. Use registerMetadata() at startup to associate type integers with
 * human-readable names and optional icon IDs.
 */
class KANOOP_EXPORT EntityMetadata
{
public:
    /** @brief Default constructor — creates an invalid (type 0) metadata object. */
    EntityMetadata() :
        _type(0), _iconId(0) {}

    /**
     * @brief Construct metadata with only a type identifier.
     * @param type Application-defined entity type integer
     */
    EntityMetadata(int type);

    /**
     * @brief Construct metadata with a type, data value, and optional role.
     * @param type Application-defined entity type integer
     * @param data Payload stored under the given role
     * @param role Model role key (default KANOOP::DataRole)
     */
    EntityMetadata(int type, const QVariant& data, KANOOP::ModelRole role = KANOOP::DataRole);

    /**
     * @brief Construct metadata with a type, data value, UUID, and optional role.
     * @param type Application-defined entity type integer
     * @param data Payload stored under the given role
     * @param uuid UUID to associate with this entity
     * @param role Model role key (default KANOOP::DataRole)
     */
    EntityMetadata(int type, const QVariant& data, const QUuid& uuid, KANOOP::ModelRole role = KANOOP::DataRole);

    /** @brief Equality comparison by type and UUID.
     * @param other EntityMetadata to compare.
     * @return True if type and UUID match. */
    bool operator==(const EntityMetadata& other) const;

    /**
     * @brief Return the entity type integer.
     * @return Type value as registered with registerMetadata()
     */
    int type() const { return _type; }

    /**
     * @brief Return the human-readable name for this entity's type.
     * @return Registered type name string, or empty string if unknown
     */
    QString typeString() const;

    /**
     * @brief Return the icon ID associated with this entity's type.
     * @return Icon ID, or 0 if none was registered
     */
    int iconId() const { return _iconId; }

    /**
     * @brief Return the data stored under the given role.
     * @param role Model role key (default KANOOP::DataRole)
     * @return Stored QVariant, or invalid QVariant if no data for role
     */
    QVariant data(int role = KANOOP::DataRole) const { return _data.value(role); }

    /**
     * @brief Store a value under the given role.
     * @param value Value to store
     * @param role Model role key (default KANOOP::DataRole)
     */
    void setData(const QVariant& value, int role = KANOOP::DataRole) { _data.insert(role, value); }

    /**
     * @brief Test whether data has been stored for a given role.
     * @param role Model role key to check
     * @return true if data exists for role
     */
    bool hasData(int role) const { return _data.contains(role); }

    /**
     * @brief Return the UUID stored under KANOOP::UUidRole.
     * @return Stored UUID, or null UUID if not set
     */
    QUuid uuid() const { return data(KANOOP::UUidRole).toUuid(); }

    /**
     * @brief Store a UUID under KANOOP::UUidRole.
     * @param value UUID to associate with this entity
     */
    void setUuid(const QUuid& value) { setData(value, KANOOP::UUidRole); }

    /**
     * @brief Wrap this object in a QVariant.
     * @return QVariant containing a copy of this EntityMetadata
     */
    QVariant toVariant() const { return  QVariant::fromValue<EntityMetadata>(*this); }

    /**
     * @brief Unwrap an EntityMetadata from a QVariant.
     * @param value Variant previously created by toVariant()
     * @return Extracted EntityMetadata
     */
    static EntityMetadata fromVariant(const QVariant& value) { return value.value<EntityMetadata>(); }

    /**
     * @brief Test whether this metadata has a non-zero type.
     * @return true if type != 0
     */
    bool isValid() const { return _type != 0; }

    /**
     * @brief Register a name (and optional icon ID) for an entity type integer.
     * @param type Application-defined entity type integer
     * @param name Human-readable name for this type
     * @param iconId Optional icon identifier to associate with this type
     */
    static void registerMetadata(int type, const QString& name, int iconId = 0);

    /**
     * @brief Associate an icon ID with an already-registered entity type.
     * @param type Registered entity type integer
     * @param iconId Icon identifier to associate
     */
    static void registerIcon(int type, int iconId);

    /**
     * @brief Look up the registered name for an entity type integer.
     * @param type Entity type integer
     * @return Registered name string, or empty string if not found
     */
    static QString getTypeString(int type);

    /**
     * @brief Look up the entity type integer for a registered name string.
     * @param value Registered name string
     * @return Matching type integer, or 0 if not found
     */
    static int getTypeFromString(const QString& value);

    /**
     * @brief Return a MIME type string for the given entity type.
     * @param type Entity type integer
     * @return MIME type string of the form "application-epcpower/<name>"
     */
    static QString getMimeTypeString(int type) { return QString("application-epcpower/%1").arg(getTypeString(type)); }

private:
    /** @brief Resolve and cache the icon ID from the registered type registry. */
    void resolveIconId();

    int _type = 0;
    QMap<int, QVariant> _data;
    int _iconId = 0;

    static QMap<int, EntityMetadataInfo*> _registeredTypes;
    static QMap<QString, int> _nameToTypeMap;
};

/**
 * @brief A list of EntityMetadata objects with type-filter support.
 */
class KANOOP_EXPORT EntityMetadataList : public QList<EntityMetadata>
{
public:
    /**
     * @brief Return all entries whose type matches the given value.
     * @param type Entity type integer to filter by
     * @return Sub-list containing only matching entries
     */
    EntityMetadataList findByType(int type) const
    {
        EntityMetadataList result;
        for(const EntityMetadata& metadata : *this) {
            if(metadata.type() == type) {
                result.append(metadata);
            }
        }
        return result;
    }

    /**
     * @brief Wrap this list in a QVariant.
     * @return QVariant containing a copy of this list
     */
    QVariant toVariant() const { return QVariant::fromValue<EntityMetadataList>(*this); }

    /**
     * @brief Unwrap an EntityMetadataList from a QVariant.
     * @param value Variant previously created by toVariant()
     * @return Extracted EntityMetadataList
     */
    static EntityMetadataList fromVariant(const QVariant& value) { return value.value<EntityMetadataList>(); }
};

Q_DECLARE_METATYPE(EntityMetadata)
Q_DECLARE_METATYPE(EntityMetadataList)

#endif // ENTITYMETADATA_H
