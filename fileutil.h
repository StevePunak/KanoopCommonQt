#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QByteArray>
#include <QString>
#include "kanoopcommon.h"

class KANOOP_EXPORT FileUtil
{
public:
    static QString getMD5String(const QString& filename);
    static QByteArray getMD5Bytes(const QString& filename);
    static bool readAllBytes(const QString& filename, QByteArray& data);
    static bool readAllLines(const QString& filename, QStringList& data);
    static bool writeAllBytes(const QString& filename, const QByteArray& data);
    static bool exists(const QString& filename);
    static bool remove(const QString& filename);
    static bool move(const QString& source, const QString& destination);
    static bool moveToDirectory(const QString& sourceFilename, const QString& destinationDirectory);
};

#endif // FILEUTIL_H
