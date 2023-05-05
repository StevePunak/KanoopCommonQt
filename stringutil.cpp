#include "stringutil.h"
#include <QTextStream>

#include <QDateTime>

QByteArray StringUtil::toByteArray(const QString &value)
{
    QByteArray result;
    if(value.contains(' ')) {
        QStringList parts = value.split(' ', Qt::SkipEmptyParts);
        for(const QString& part : parts) {
            bool ok;
            uint32_t val = part.toUInt(&ok, 16);
            if(ok) {
                result.append(val);
            }
        }
    }
    else {
        result = QByteArray::fromHex(value.toUtf8());
    }
    return result;
}

QString StringUtil::toString(double value, int precision)
{
    QString result = QString("%1").arg(value, 0, 'f', precision);
    result = trimEnd(result, QList<QChar>() << '0');
    if(result.endsWith('.')) {
        result = trimEnd(result, QList<QChar>() << '.');
    }
    return result;
}

QString StringUtil::toString(const QByteArray &value, const QString &delimiter)
{
    QString result;
    QTextStream output(&result);
    for(int i = 0;i < value.length();i++) {
        quint8 b = value.at(i);
        output << QString::number(b, 16).toUpper();
        if(delimiter.length() > 0) {
            output << delimiter;
        }
    }
    return result.trimmed();
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

int StringUtil::fuzzyIndexOf(const QString &needle, const QString &haystack, int maxDistance)
{
    Bitap bitap(haystack, needle, maxDistance);
    return bitap.index();
}

int StringUtil::levenshteinDistance(const QString &s1, const QString &s2)
{
    Levenshtein l(s1, s2);
    return l.distance();
}

StringUtil::Levenshtein::Levenshtein(const QString &s1, const QString &s2)
{
    unsigned int x, y, s1len, s2len;
    s1len = s1.length();
    s2len = s2.length();
    unsigned int matrix[s2len+1][s1len+1];
    matrix[0][0] = 0;
    for (x = 1; x <= s2len; x++) {
        matrix[x][0] = matrix[x-1][0] + 1;
    }
    for (y = 1; y <= s1len; y++) {
        matrix[0][y] = matrix[0][y-1] + 1;
    }
    for (x = 1; x <= s2len; x++) {
        for (y = 1; y <= s1len; y++) {
            matrix[x][y] = min(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));
        }
    }
    _distance = matrix[s2len][s1len];
}

StringUtil::Bitap::Bitap(const QString &haystack, const QString &needle, int maxDistance) :
    _haystack(haystack), _needle(needle)
{
    _index = Bitap::calculate(maxDistance);
}

int StringUtil::Bitap::calculate(int maxDistance)
{
    int result = -1;
    int m = _needle.size();
    unsigned long *R;
    unsigned long patternMask[CHAR_MAX + 1];
    int i, d;

    if (_needle[0] == '\0') return 0;
    if (m > 31) return -1; //Error: The pattern is too long!

    R = new unsigned long[(maxDistance + 1) * sizeof *R];
    for (i = 0; i <= maxDistance; ++i)
        R[i] = ~1;

    for (i = 0; i <= CHAR_MAX; ++i)
        patternMask[i] = ~0;

    for (i = 0; i < m; ++i)
        patternMask[(int)_needle[i].toLatin1()] &= ~(1UL << i);

    for (i = 0; i < _haystack.length(); ++i)
    {
        unsigned long oldRd1 = R[0];

        R[0] |= patternMask[(int)_haystack[i].toLatin1()];
        R[0] <<= 1;

        for (d = 1; d <= maxDistance; ++d)
        {
            unsigned long tmp = R[d];

            R[d] = (oldRd1 & (R[d] | patternMask[(int)_haystack[i].toLatin1()])) << 1;
            oldRd1 = tmp;
        }

        if (0 == (R[maxDistance] & (1UL << m)))
        {
            result = (i - m) + 1;
            break;
        }
    }

    delete[] R;
    return result;
}
