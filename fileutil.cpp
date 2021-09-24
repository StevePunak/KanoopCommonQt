#include "fileutil.h"
#include "pathutil.h"
#include "cryptoutil.h"

#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>

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
    return CryptoUtil::fileMd5(filename);
}

bool FileUtil::readAllBytes(const QString &filename, QByteArray &data)
{
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        data = file.readAll();
        return true;
    }
    return false;
}

bool FileUtil::readAllLines(const QString &filename, QStringList &lines)
{
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream ts(&file);
        QString line;
        while((line = ts.readLine()).isNull() == false)
        {
            lines.append(line);
        }
        return true;
    }
    return false;
}

bool FileUtil::writeAllBytes(const QString &filename, const QByteArray &data)
{
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(data);
        return true;
    }
    return false;
}

bool FileUtil::writeAllLines(const QString &filename, const QStringList &lines)
{
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        foreach(const QString& line, lines)
        {
            file.write(line.toLatin1());
            file.write("\n");
        }
        return true;
    }
    return false;
}

bool FileUtil::exists(const QString &filename)
{
    QFile file(filename);
    return file.exists();
}

bool FileUtil::remove(const QString &filename)
{
    return QFile(filename).remove();
}

bool FileUtil::move(const QString &source, const QString &destination)
{
    bool result = false;
    QFile currentFile(source);
    if(currentFile.exists())
    {
        result = currentFile.rename(destination);
    }
    return result;
}

bool FileUtil::moveToDirectory(const QString &sourceFilename, const QString &destinationDirectory)
{
    bool result = false;
    QFile currentFile(sourceFilename);
    if(currentFile.exists())
    {
        QFileInfo currentInfo(sourceFilename);
        QString destFilename = PathUtil::combine(destinationDirectory, currentInfo.fileName());
        result = currentFile.rename(destFilename);
    }
    return result;
}
