#include "cryptoutil.h"

#include <QCryptographicHash>
#include <QFile>

QByteArray CryptoUtil::fileMd5(const QString &filename)
{
    QFile file(filename);
    QByteArray result;
    if(file.open(QFile::ReadOnly))
    {
        QCryptographicHash hash(QCryptographicHash::Md5);
        char buffer[65536];
        int bytesRead;
        while((bytesRead = file.read(buffer, sizeof(buffer))) > 0)
            hash.addData(buffer, bytesRead);
        result = hash.result();
    }
    return result;
}

QString CryptoUtil::fileMd5String(const QString &filename)
{
    QByteArray hash = fileMd5(filename);
    return toHashString(hash);
}

QByteArray CryptoUtil::md5(const QByteArray &of)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(of);
    return hash.result();
}

QString CryptoUtil::md5String(const QString &of)
{
    QByteArray hash = md5(of.toLatin1());
    return toHashString(hash);
}

QString CryptoUtil::md5String(const QStringList &of)
{
    return toHashString(md5(of));
}

QByteArray CryptoUtil::md5(const QStringList &of)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    foreach(const QString& line, of)
        hash.addData(line.toLatin1());
    return hash.result();
}

QString CryptoUtil::toHashString(const QByteArray &buffer)
{
    QString result;
    if(buffer.length() > 0)
    {
        result.reserve(32);
        for(int x = 0;x < buffer.length();x++)
            result.append(QStringLiteral("%1").arg((quint8)buffer.constData()[x], 2, 16, QLatin1Char('0')));
    }
    return result;
}
