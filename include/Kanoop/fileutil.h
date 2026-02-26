/**
 *  FileUtil
 *
 *  Some static helper methods for one line file reads / writes and other
 *  file manipulations.
 *
 *  Stephen Punak, October 02 2020
 */
#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QByteArray>
#include <QDateTime>
#include <QString>
#include "kanoopcommon.h"

/**
 * @brief Static helper methods for common file I/O operations.
 */
class KANOOP_EXPORT FileUtil
{
public:
    /**
     * @brief Compute the MD5 hash of a file as a hex string.
     * @param filename Path to the file
     * @return Hex-encoded MD5 string, or empty string on error
     */
    static QString getMD5String(const QString& filename);

    /**
     * @brief Compute the MD5 hash of a file as raw bytes.
     * @param filename Path to the file
     * @return Raw MD5 hash bytes, or empty array on error
     */
    static QByteArray getMD5Bytes(const QString& filename);

    /**
     * @brief Read the entire contents of a file into a byte array.
     * @param filename Path to the file
     * @param data Output byte array receiving file contents
     * @return true on success, false on error
     */
    static bool readAllBytes(const QString& filename, QByteArray& data);

    /**
     * @brief Read all lines of a text file into a string list.
     * @param filename Path to the file
     * @param data Output string list receiving the lines
     * @param encoding Text encoding to use (default UTF-8)
     * @return true on success, false on error
     */
    static bool readAllLines(const QString& filename, QStringList& data, QStringConverter::Encoding encoding = QStringConverter::Utf8);

    /**
     * @brief Write a byte array to a file, overwriting any existing content.
     * @param filename Path to the output file
     * @param data Bytes to write
     * @return true on success, false on error
     */
    static bool writeAllBytes(const QString& filename, const QByteArray& data);

    /**
     * @brief Write a byte array to a file using a specific text encoding.
     * @param filename Path to the output file
     * @param data Bytes to write
     * @param encoding Text encoding used when writing
     * @return true on success, false on error
     */
    static bool writeAllBytes(const QString& filename, const QByteArray& data, QStringConverter::Encoding encoding);

    /**
     * @brief Write a list of strings to a file, one per line.
     * @param filename Path to the output file
     * @param lines Lines to write
     * @return true on success, false on error
     */
    static bool writeAllLines(const QString& filename, const QStringList& lines);

    /**
     * @brief Append a byte array to an existing file (or create if absent).
     * @param filename Path to the file
     * @param data Bytes to append
     * @return true on success, false on error
     */
    static bool appendAllBytes(const QString& filename, const QByteArray& data);

    /**
     * @brief Count the number of lines in a text file.
     * @param filename Path to the file
     * @return Number of lines, or -1 on error
     */
    static int lineCount(const QString& filename);

    /**
     * @brief Test whether a file exists.
     * @param filename Path to the file
     * @return true if the file exists
     */
    static bool exists(const QString& filename);

    /**
     * @brief Remove (delete) a file.
     * @param filename Path to the file to remove
     * @return true on success, false on error
     */
    static bool remove(const QString& filename);

    /**
     * @brief Create an empty file or update its modification time if it exists.
     * @param filename Path to the file
     * @return true on success, false on error
     */
    static bool touch(const QString& filename);

    /**
     * @brief Move (rename) a file from one path to another.
     * @param source Path to the source file
     * @param destination Path to the destination
     * @return true on success, false on error
     */
    static bool move(const QString& source, const QString& destination);

    /**
     * @brief Move a file into a destination directory, preserving its basename.
     * @param sourceFilename Path to the source file
     * @param destinationDirectory Path to the target directory
     * @return true on success, false on error
     */
    static bool moveToDirectory(const QString& sourceFilename, const QString& destinationDirectory);

    /**
     * @brief Set the modification timestamp of a file.
     * @param filename Path to the file
     * @param value New modification timestamp
     * @return true on success, false on error
     */
    static bool setModifyTime(const QString& filename, const QDateTime& value);

    /**
     * @brief Generate a unique temporary file path.
     * @return Path string for a new temporary file
     */
    static QString getTempFilename();
};

#endif // FILEUTIL_H
