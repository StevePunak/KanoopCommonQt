/**
 * @brief A 2D size extending QSizeF with grow/shrink helpers and Rectangle conversion.
 */
#ifndef SIZE_H
#define SIZE_H
#include <QSizeF>
#include "Kanoop/kanoopcommon.h"

class Rectangle;

/**
 * @brief Extends QSizeF with convenient grow/shrink operations and Rectangle conversion.
 */
class KANOOP_EXPORT Size : public QSizeF
{
public:
    /** @brief Default constructor — creates a null (0x0) size. */
    Size() : QSizeF() {}
    /** @brief Construct from a QSizeF.
     * @param other Source QSizeF */
    Size(const QSizeF& other) : QSizeF(other) {}
    /** @brief Construct from a QSize (integer dimensions).
     * @param other Source QSize */
    Size(const QSize& other) : QSizeF(other) {}
    /**
     * @brief Construct from explicit width and height.
     * @param w Width
     * @param h Height
     */
    Size(double w, double h) : QSizeF(w, h) {}

    /**
     * @brief Convert this size to a Rectangle positioned at the origin.
     * @return Rectangle(0, 0, width, height)
     */
    Rectangle toRectangle() const;

    /**
     * @brief Grow both dimensions by the given amount.
     * @param amount Amount to add to width and height
     */
    void grow(double amount);

    /**
     * @brief Shrink both dimensions by the given amount.
     * @param amount Amount to subtract from width and height
     */
    void shrink(double amount);

    /**
     * @brief Format this size as "w, h".
     * @return String representation
     */
    QString toString() const { return QString("%1, %2").arg(width()).arg(height()); }

    /**
     * @brief Parse a Size from a string produced by toString().
     * @param value String of the form "w, h"
     * @return Parsed Size
     */
    static Size fromString(const QString& value);
};

#endif // SIZE_H
