/**
 * @brief A lookup table for commonly used Unicode special characters.
 */
#ifndef UNICODE_H
#define UNICODE_H

#include <QMap>
#include <Kanoop/kanoopcommon.h>

/**
 * @brief Provides QChar values for common Unicode special characters by name.
 *
 * Use the SpecialCharacter enumeration with specialCharacter() to retrieve
 * the corresponding QChar, e.g. arrows, mathematical symbols, and typographic marks.
 */
class KANOOP_EXPORT Unicode
{
public:
    /**
     * @brief Enumeration of supported Unicode special characters.
     */
    enum SpecialCharacter
    {
        ArrowDown,          ///< Downward arrow (↓, U+2193)
        ArrowDownToLeft,    ///< Down-left arrow (↙, U+2199)
        ArrowDownToRight,   ///< Down-right arrow (↙, U+2199)
        ArrowHorizontalBoth,///< Left-right arrow (↔, U+2194)
        ArrowLeft,          ///< Leftward arrow (←, U+2190)
        ArrowRight,         ///< Rightward arrow (→, U+2192)
        ArrowUp,            ///< Upward arrow (↑, U+2191)
        ArrowUpToLeft,      ///< Up-left arrow (↖, U+2196)
        ArrowUpToRight,     ///< Up-right arrow (↗, U+2197)
        ArrowVerticalBoth,  ///< Up-down arrow (↕, U+2195)
        Bullet,             ///< Middle dot / bullet (·, U+00B7)
        Copyright,          ///< Copyright sign (©, U+00A9)
        Cubed,              ///< Superscript three (³, U+00B3)
        Degrees,            ///< Degree sign (°, U+00B0)
        Delta,              ///< Greek capital Delta (Δ, U+0394)
        Micro,              ///< Micro sign (µ, U+00B5)
        OneHalf,            ///< Vulgar fraction one half (½, U+00BD)
        OneQuarter,         ///< Vulgar fraction one quarter (¼, U+00BC)
        Paragraph,          ///< Pilcrow / paragraph sign (¶, U+00B6)
        PlusOrMinus,        ///< Plus-minus sign (±, U+00B1)
        RightsReserved,     ///< Registered sign (®, U+00AE)
        Squared,            ///< Superscript two (², U+00B2)
        ThreeQuarters,      ///< Vulgar fraction three quarters (¾, U+00BE)
    };

    /**
     * @brief Return the QChar for a given special character identifier.
     * @param c Special character to look up
     * @return Corresponding QChar, or a null QChar if not found
     */
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
            insert(Delta,                   QChar(0x0394));
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
