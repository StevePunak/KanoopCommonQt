/**
 * @brief Interface for objects that can serialize to and deserialize from a QDataStream.
 */
#ifndef ISERIALIZABLEINTODATASTREAM_H
#define ISERIALIZABLEINTODATASTREAM_H
#include <Kanoop/kanoopcommon.h>

/**
 * @brief Interface for binary serialization via QDataStream.
 *
 * Implement both serializeIntoDataStream() and deserializeFromDataStream() to
 * support round-trip binary encoding compatible with Qt's data stream format.
 */
class ISerializableIntoDataStream
{
public:
    /**
     * @brief Populate this object by reading from a QDataStream.
     * @param stream Source data stream positioned at the start of this object's data
     */
    virtual void deserializeFromDataStream(QDataStream& stream) = 0;

    /**
     * @brief Write this object's state into a QDataStream.
     * @param stream Destination data stream to write to
     */
    virtual void serializeIntoDataStream(QDataStream& stream) const = 0;
};

#endif // ISERIALIZABLEINTODATASTREAM_H
