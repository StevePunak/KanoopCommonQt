/**
 *  CryptoUtil
 *
 *  Some static helper methods for MD5 hashes (to and from strings).
 *
 *  It is basically a front-end for QCryptographicHash functions.
 *
 *  Stephen Punak, February 17 2021
 */
#ifndef CRYPTOUTIL_H
#define CRYPTOUTIL_H

#include <QByteArray>
#include <QCryptographicHash>
#include <QString>
#include <QStringList>
#include <Kanoop/kanoopcommon.h>

/**
 * @brief Static helper methods for computing MD5 hashes as byte arrays or hex strings.
 */
class KANOOP_EXPORT CryptoUtil
{
public:
    /** @brief Hash algorithms supported by CryptoUtil. */
    enum Algorithm { Md5, Sha256 };

    /**
     * @brief Compute the MD5 hash of a file's contents.
     * @param filename Path to the file
     * @return Raw MD5 hash bytes, or empty array on error
     */
    static QByteArray fileMd5(const QString& filename);

    /**
     * @brief Compute the MD5 hash of a file's contents as a hex string.
     * @param filename Path to the file
     * @return Hex-encoded MD5 string, or empty string on error
     */
    static QString fileMd5String(const QString& filename);

    /**
     * @brief Compute the MD5 hash of a byte array.
     * @param of Input data
     * @return Raw MD5 hash bytes
     */
    static QByteArray md5(const QByteArray& of);

    /**
     * @brief Compute the MD5 hash of a byte array as a 64-bit unsigned integer.
     * @param of Input data
     * @return First 8 bytes of the MD5 hash interpreted as uint64_t
     */
    static uint64_t md5UInt64(const QByteArray& of);

    /**
     * @brief Compute the MD5 hash of a string as a hex string.
     * @param of Input string
     * @return Hex-encoded MD5 string
     */
    static QString md5String(const QString& of);

    /**
     * @brief Compute the MD5 hash of a byte array as a hex string.
     * @param of Input data
     * @return Hex-encoded MD5 string
     */
    static QString md5String(const QByteArray& of);

    /**
     * @brief Compute the MD5 hash of a joined string list as a hex string.
     * @param of List of strings to hash (concatenated)
     * @return Hex-encoded MD5 string
     */
    static QString md5String(const QStringList& of);

    /**
     * @brief Compute the MD5 hash of a string list as raw bytes.
     * @param of List of strings to hash (concatenated)
     * @return Raw MD5 hash bytes
     */
    static QByteArray md5(const QStringList& of);

    /**
     * @brief Compute the SHA-256 hash of a file's contents.
     * @param filename Path to the file
     * @return Raw SHA-256 hash bytes, or empty array on error
     */
    static QByteArray fileSha256(const QString& filename);

    /**
     * @brief Compute the SHA-256 hash of a file's contents as a hex string.
     * @param filename Path to the file
     * @return Hex-encoded SHA-256 string, or empty string on error
     */
    static QString fileSha256String(const QString& filename);

    /**
     * @brief Compute the SHA-256 hash of a byte array.
     * @param of Input data
     * @return Raw SHA-256 hash bytes
     */
    static QByteArray sha256(const QByteArray& of);

    /**
     * @brief Compute the SHA-256 hash of a string as a hex string.
     * @param of Input string
     * @return Hex-encoded SHA-256 string
     */
    static QString sha256String(const QString& of);

    /**
     * @brief Compute the SHA-256 hash of a byte array as a hex string.
     * @param of Input data
     * @return Hex-encoded SHA-256 string
     */
    static QString sha256String(const QByteArray& of);

    /**
     * @brief Compute the SHA-256 hash of a joined string list as a hex string.
     * @param of List of strings to hash (concatenated)
     * @return Hex-encoded SHA-256 string
     */
    static QString sha256String(const QStringList& of);

    /**
     * @brief Compute the SHA-256 hash of a string list as raw bytes.
     * @param of List of strings to hash (concatenated)
     * @return Raw SHA-256 hash bytes
     */
    static QByteArray sha256(const QStringList& of);

    /**
     * @brief Stateful incremental hasher for streamed input.
     *
     * For cases where the full input isn't available up front (e.g. streaming
     * network downloads). Hex output matches the static md5String/sha256String
     * helpers in this class.
     */
    class KANOOP_EXPORT Hasher
    {
    public:
        /**
         * @brief Construct a hasher for the given algorithm.
         * @param algorithm Which digest to compute.
         */
        explicit Hasher(Algorithm algorithm);

        /**
         * @brief Append data to the running hash.
         * @param data Bytes to fold into the digest.
         */
        void addData(const QByteArray& data);

        /** @brief Reset the hash state so the instance can be reused. */
        void reset();

        /** @brief Return the current digest as raw bytes. */
        QByteArray result() const;

        /** @brief Return the current digest as a lowercase hex string. */
        QString resultString() const;

    private:
        QCryptographicHash _hash;
    };

private:
    /** @brief Convert a raw hash byte array to a lowercase hex string. */
    static QString toHashString(const QByteArray& buffer);
};

#endif // CRYPTOUTIL_H
