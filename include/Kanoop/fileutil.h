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

class KANOOP_EXPORT FileUtil
{
public:
    static QString getMD5String(const QString& filename);
    static QByteArray getMD5Bytes(const QString& filename);
    static bool readAllBytes(const QString& filename, QByteArray& data);
    static bool readAllLines(const QString& filename, QStringList& data, QStringConverter::Encoding encoding = QStringConverter::Utf8);
    static bool writeAllBytes(const QString& filename, const QByteArray& data);
    static bool writeAllBytes(const QString& filename, const QByteArray& data, QStringConverter::Encoding encoding);
    static bool writeAllLines(const QString& filename, const QStringList& lines);
    static bool appendAllBytes(const QString& filename, const QByteArray& data);
    static int lineCount(const QString& filename);
    static bool exists(const QString& filename);
    static bool remove(const QString& filename);
    static bool touch(const QString& filename);
    static bool move(const QString& source, const QString& destination);
    static bool moveToDirectory(const QString& sourceFilename, const QString& destinationDirectory);
    static bool setModifyTime(const QString& filename, const QDateTime& value);
    static QString getTempFilename();
};

#endif // FILEUTIL_H
