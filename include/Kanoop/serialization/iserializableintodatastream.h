#ifndef ISERIALIZABLEINTODATASTREAM_H
#define ISERIALIZABLEINTODATASTREAM_H
#include <Kanoop/kanoopcommon.h>

class ISerializableIntoDataStream
{
public:
    virtual void deserializeFromDataStream(QDataStream& stream) = 0;
    virtual void serializeIntoDataStream(QDataStream& stream) const = 0;
};

#endif // ISERIALIZABLEINTODATASTREAM_H
