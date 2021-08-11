#include "stringutil.h"
#include <QTextStream>

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
