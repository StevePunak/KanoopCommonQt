#ifndef UNICODE_H
#define UNICODE_H

#include <QMap>
#include <Kanoop/kanoopcommon.h>


class KANOOP_EXPORT Unicode
{
public:
    enum SpecialCharacter
    {
        ArrowDown,
        ArrowDownToLeft,
        ArrowDownToRight,
        ArrowHorizontalBoth,
        ArrowLeft,
        ArrowRight,
        ArrowUp,
        ArrowUpToLeft,
        ArrowUpToRight,
        ArrowVerticalBoth,
        Bullet,
        Copyright,
        Cubed,
        Degrees,
        Micro,
        OneHalf,
        OneQuarter,
        Paragraph,
        PlusOrMinus,
        RightsReserved,
        Squared,
        ThreeQuarters,
    };

    static QChar specialCharacter(SpecialCharacter c) { return _SpecialCharacterMap.value(c, QChar()); }

private:
    class SpecialCharacterMap : public QMap<SpecialCharacter, QChar>
    {
    public:
        SpecialCharacterMap()
        {
            insert(ArrowDown,               QChar(0x2193));
            insert(ArrowDownToLeft,         QChar(0x2199));
            insert(ArrowDownToRight,        QChar(0x2199));
            insert(ArrowHorizontalBoth,     QChar(0x2194));
            insert(ArrowLeft,               QChar(0x2190));
            insert(ArrowRight,              QChar(0x2192));
            insert(ArrowUp,                 QChar(0x2191));
            insert(ArrowUpToLeft,           QChar(0x2196));
            insert(ArrowUpToRight,          QChar(0x2197));
            insert(ArrowVerticalBoth,       QChar(0x2195));
            insert(Bullet,                  QChar(0x00b7));
            insert(Copyright,               QChar(0x00a9));
            insert(Cubed,                   QChar(0x00b3));
            insert(Degrees,                 QChar(0x00b0));
            insert(Micro,                   QChar(0x00b5));
            insert(OneHalf,                 QChar(0x00bd));
            insert(OneQuarter,              QChar(0x00bc));
            insert(Paragraph,               QChar(0x00b6));
            insert(PlusOrMinus,             QChar(0x00b1));
            insert(RightsReserved,          QChar(0x00ae));
            insert(Squared,                 QChar(0x00b2));
            insert(ThreeQuarters,           QChar(0x00be));
        }
    };
    static const SpecialCharacterMap _SpecialCharacterMap;
};

#endif // UNICODE_H
