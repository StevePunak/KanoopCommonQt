#include "stringutil.h"
#include <QTextStream>


QString StringUtil::toString(double value, int precision)
{
    QString result = QString("%1").arg(value, 0, 'f', precision);
    result = trimEnd(result, QList<QChar>() << '0');
    if(result.endsWith('.')) {
        result = trimEnd(result, QList<QChar>() << '.');
    }
    return result;
}

QString StringUtil::toDelimitedString(const QStringList &list, char delimiter)
{
    QString outputString;
    outputString.reserve(list.size() * 256);
    QTextStream output(&outputString);
    for(int i = 0;i < list.count();i++)
    {
        output << list.at(i);
        if(i < list.count() - 1)
            output << delimiter;
    }
    return outputString;
}

QString StringUtil::toKMG(qint64 byteCount)
{
    QString result;
    if(byteCount >= 1000000000)
    {
        // format GIGs
        result = QString("%1 G").arg((double)((double)byteCount / 1000000000), 0, 'f', 2);
    }
    else if(byteCount >= 1000000)
    {
        // format MEGs
        result = QString("%1 M").arg((double)((double)byteCount / 1000000), 0, 'f', 2);
    }
    else if(byteCount >= 1000)
    {
        // format K
        result = QString("%1 K").arg((double)((double)byteCount / 1000), 0, 'f', 2);
    }
    else
    {
        result = QString("%1 ").arg(byteCount);
    }
    return result;
}

QString StringUtil::trimFront(const QString &value, const QList<QChar> &chars)
{
    QString result = value;
    while(result.length() > 0) {
        QChar first = result.at(0);
        if(chars.contains(first)) {
            result = result.mid(1);
        }
        else {
            break;
        }
    }
    return result;
}

QString StringUtil::trimEnd(const QString& value, const QList<QChar> &chars)
{
    QString result = value;
    while(result.length() > 0) {
        QChar last = result.at(result.length() - 1);
        if(chars.contains(last)) {
            result = result.mid(0, result.length() - 1);
        }
        else {
            break;
        }
    }
    return result;
}

QString StringUtil::trimBothEnds(const QString &value, const QList<QChar> &chars)
{
    QString result = trimFront(value, chars);
    result = trimEnd(result, chars);
    return result;
}

