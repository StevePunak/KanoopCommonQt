/**
 * @brief Interface for objects that can serialize to and deserialize from a QByteArray.
 */
#ifndef ISERIALIZABLETOBYTEARRAY_H
#define ISERIALIZABLETOBYTEARRAY_H
#include <Kanoop/kanoopcommon.h>

/**
 * @brief Interface for arbitrary binary serialization into a QByteArray.
 *
 * Implement both serializeToByteArray() and deserializeFromByteArray() to
 * support round-trip encoding in an application-defined binary format.
 */
class ISerializableToByteArray
{
public:
    /**
     * @brief Populate this object by reading from a raw byte array.
     * @param data Previously produced by serializeToByteArray()
     */
    virtual void deserializeFromByteArray(const QByteArray& data) = 0;

    /**
     * @brief Serialize this object into a raw byte array.
     * @return Byte array representation of this object's state
     */
    virtual QByteArray serializeToByteArray() const = 0;
};

#endif // ISERIALIZABLETOBYTEARRAY_H
