#ifndef CRYPTOUTIL_H
#define CRYPTOUTIL_H

#include <QByteArray>
#include <QString>
#include <QStringList>
#include <Kanoop/kanoopcommon.h>

class KANOOP_EXPORT CryptoUtil
{
public:
    static QByteArray fileMd5(const QString& filename);
    static QString fileMd5String(const QString& filename);
    static QByteArray md5(const QByteArray& of);
    static uint64_t md5UInt64(const QByteArray& of);
    static QString md5String(const QString& of);
    static QString md5String(const QByteArray& of);
    static QString md5String(const QStringList& of);
    static QByteArray md5(const QStringList& of);

private:
    static QString toHashString(const QByteArray& buffer);
};

#endif // CRYPTOUTIL_H
