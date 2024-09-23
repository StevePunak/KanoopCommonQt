#include "entitymetadata.h"

#include "entitymetadatainfo.h"

#include <QMutex>
#include <Kanoop/log.h>

QMap<int, EntityMetadataInfo*> EntityMetadata::_registeredTypes;

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

void EntityMetadata::resolveIconId()
{
    EntityMetadataInfo* metadataInfo = _registeredTypes.value(_type);
    if(metadataInfo != nullptr) {
        _iconId = metadataInfo->iconId();
    }
    else {
        Log::logText(LVL_WARNING, "Failed to find metadata into");
    }
}
