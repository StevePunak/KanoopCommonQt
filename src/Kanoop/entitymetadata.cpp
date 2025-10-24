#include "entitymetadata.h"

#include "entitymetadatainfo.h"

#include <QMutex>
#include <Kanoop/log.h>

QMap<int, EntityMetadataInfo*> EntityMetadata::_registeredTypes;
QMap<QString, int> EntityMetadata::_nameToTypeMap;

EntityMetadata::EntityMetadata(int type) :
    _type(type), _iconId(0)
{
    resolveIconId();
}

EntityMetadata::EntityMetadata(int type, const QVariant &data, KANOOP::ModelRole role) :
    _type(type), _iconId(0)
{
    _data[role] = data;
    resolveIconId();
}

EntityMetadata::EntityMetadata(int type, const QVariant& data, const QUuid& uuid, KANOOP::ModelRole role) :
    _type(type), _iconId(0)
{
    _data[role] = data;
    resolveIconId();
    setUuid(uuid);
}

bool EntityMetadata::operator==(const EntityMetadata& other) const
{
    bool result = _type == other._type &&
                  _iconId == other._iconId &&
                  _data.count() == other._data.count();
    if(result == true) {
        QList<int> keys = _data.keys();
        for(int i = 0;i < keys.count();i++) {
            int key = keys.at(i);
            if((result = _data[key] == other._data[key]) == false) {
                break;
            }
        }
    }
    return result;
}

QString EntityMetadata::typeString() const
{
    return getTypeString(_type);
}

void EntityMetadata::registerMetadata(int type, const QString &name, int iconId)
{
    static QMutex _lock;
    _lock.lock();
    EntityMetadataInfo* metadataInfo = _registeredTypes.value(type);
    if(metadataInfo != nullptr && metadataInfo->name() != name) {
        Log::logText(LVL_WARNING, QString("Replacing metadata type %1").arg(name));
        delete metadataInfo;
    }
    else if(metadataInfo == nullptr) {
        _registeredTypes.insert(type, new EntityMetadataInfo(type, name, iconId));
        _nameToTypeMap.insert(name, type);
    }
    _lock.unlock();
}

void EntityMetadata::registerIcon(int type, int iconId)
{
    EntityMetadataInfo* metadataInfo = _registeredTypes.value(type);
    if(metadataInfo != nullptr) {
        metadataInfo->setIconId(iconId);
    }
    else {
        Log::logText(LVL_WARNING, "Failed to find metadata into");
    }
}

QString EntityMetadata::getTypeString(int type)
{
    QString result;
    EntityMetadataInfo* metadataInfo = _registeredTypes.value(type);
    if(metadataInfo != nullptr) {
        result = metadataInfo->name();
    }
    return result;
}

int EntityMetadata::getTypeFromString(const QString& value)
{
    int result = _nameToTypeMap.value(value);
    return result;
}

void EntityMetadata::resolveIconId()
{
    EntityMetadataInfo* metadataInfo = _registeredTypes.value(_type);
    if(metadataInfo != nullptr) {
        _iconId = metadataInfo->iconId();
    }
}
