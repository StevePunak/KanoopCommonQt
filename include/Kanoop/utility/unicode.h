#ifndef UNICODE_H
#define UNICODE_H

#include <QMap>
#include <Kanoop/kanoopcommon.h>


class KANOOP_EXPORT Unicode
{
public:
    enum SpecialCharacter
    {
        ArrowUp,
        ArrowDown,
        ArrowLeft,
        ArrowRight,
        ArrowHorizontalBoth,
        ArrowVerticalBoth,
        ArrowUpToLeft,
        ArrowUpToRight,
        ArrowDownToLeft,
        ArrowDownToRight,
        Degrees,
    };

    static QChar specialCharacter(SpecialCharacter c) { return _SpecialCharacterMap.value(c, QChar()); }

private:
    class SpecialCharacterMap : public QMap<SpecialCharacter, QChar>
    {
    public:
        SpecialCharacterMap()
        {
            insert(ArrowUp,                 QChar(0x2191));
            insert(ArrowDown,               QChar(0x2193));
            insert(ArrowLeft,               QChar(0x2190));
            insert(ArrowRight,              QChar(0x2192));
            insert(ArrowHorizontalBoth,     QChar(0x2194));
            insert(ArrowVerticalBoth,       QChar(0x2195));
            insert(ArrowUpToLeft,           QChar(0x2196));
            insert(ArrowUpToRight,          QChar(0x2197));
            insert(ArrowDownToLeft,         QChar(0x2199));
            insert(ArrowDownToRight,        QChar(0x2199));
            insert(Degrees,                 QChar(0x00b0));
        }
    };
    static const SpecialCharacterMap _SpecialCharacterMap;
};

#endif // UNICODE_H
