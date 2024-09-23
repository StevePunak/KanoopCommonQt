#include "entitymetadata.h"

#include "entitymetadatainfo.h"

#include <QMutex>
#include <Kanoop/log.h>

QMap<int, EntityMetadataInfo> EntityMetadata::_registeredTypes;

void EntityMetadata::registerMetadata(int type, const QString &name)
{
    static QMutex _lock;
    _lock.lock();
    if(_registeredTypes.contains(type) && _registeredTypes.value(type).name() != name) {
        Log::logText(LVL_WARNING, QString("Replacing metadata type %1").arg(name));
    }
    _registeredTypes.insert(type, EntityMetadataInfo(type, name));
    _lock.unlock();
}
