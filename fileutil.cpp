#include "fileutil.h"

#include <QCryptographicHash>
#include <QFile>

QString FileUtil::getMD5String(const QString &filename)
{
    QByteArray hash = getMD5Bytes(filename);
    QString result;
    if(hash.length() > 0)
    {
        result.reserve(32);
        for(int x = 0;x < hash.length();x++)
            result.append(QStringLiteral("%1").arg((quint8)hash.constData()[x], 2, 16, QLatin1Char('0')));
    }
    return result;
}

QByteArray FileUtil::getMD5Bytes(const QString &filename)
{
    QFile f(filename);
    QByteArray result;
    if (f.open(QFile::ReadOnly))
    {
        QCryptographicHash hash(QCryptographicHash::Md5);
        char buffer[65536];
        int bytesRead;
        while((bytesRead = f.read(buffer, sizeof(buffer))) > 0)
            hash.addData(buffer, bytesRead);
        result = hash.result();
    }
    return result;
}
