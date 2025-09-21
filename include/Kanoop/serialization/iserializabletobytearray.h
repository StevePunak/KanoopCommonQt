#ifndef ISERIALIZABLETOBYTEARRAY_H
#define ISERIALIZABLETOBYTEARRAY_H
#include <Kanoop/kanoopcommon.h>

class KANOOP_EXPORT ISerializableToByteArray
{
public:
    virtual void deserializeFromByteArray(const QByteArray& data) = 0;
    virtual QByteArray serializeToByteArray() const = 0;
};

#endif // ISERIALIZABLETOBYTEARRAY_H
