#ifndef ISERIALIZABLETOBYTEARRAY_H
#define ISERIALIZABLETOBYTEARRAY_H
#include <Kanoop/kanoopcommon.h>

class ISerializableToByteArray
{
public:
    virtual void deserializeFromByteArray(const QByteArray& data) = 0;
    virtual QByteArray serializeToByteArray() const = 0;
};

#endif // ISERIALIZABLETOBYTEARRAY_H
