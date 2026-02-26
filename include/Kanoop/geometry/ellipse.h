/**
 * @brief A 2D ellipse defined by a centre point and two semi-axis lengths.
 */
#ifndef ELLIPSE_H
#define ELLIPSE_H
#include <Kanoop/geometry/point.h>

/**
 * @brief Represents an axis-aligned 2D ellipse with semi-axes A (horizontal) and B (vertical).
 */
class KANOOP_EXPORT Ellipse
{
public:
    /** @brief Default constructor — creates a degenerate ellipse at the origin. */
    Ellipse() {}

    /**
     * @brief Construct an ellipse from a centre point and semi-axis lengths.
     * @param center Centre of the ellipse
     * @param semiAxisA Half-length of the horizontal axis
     * @param semiAxisB Half-length of the vertical axis
     */
    Ellipse(const Point& center, double semiAxisA, double semiAxisB) :
        _center(center), _semiAxisA(semiAxisA), _semiAxisB(semiAxisB) {}

    /**
     * @brief Return the centre of the ellipse.
     * @return Centre Point
     */
    Point center() const { return _center; }

    /**
     * @brief Set the centre of the ellipse.
     * @param value New centre point
     */
    void setCenter(const Point& value) { _center = value; }

    /**
     * @brief Return the horizontal semi-axis length.
     * @return Semi-axis A (horizontal half-width)
     */
    double semiAxisA() const { return  _semiAxisA; }

    /**
     * @brief Set the horizontal semi-axis length.
     * @param value New semi-axis A value
     */
    void setSemiAxisA(double value) { _semiAxisA = value; }

    /**
     * @brief Return the vertical semi-axis length.
     * @return Semi-axis B (vertical half-height)
     */
    double semiAxisB() const { return  _semiAxisB; }

    /**
     * @brief Set the vertical semi-axis length.
     * @param value New semi-axis B value
     */
    void setSemiAxisB(double value) { _semiAxisB = value; }

    /**
     * @brief radiusAtAngle
     * @param angle in degrees offset from vertical (north) at zero
     * @return The radius at the given angle.
     */
    double radiusAtAngle(double angle) const;

    /**
     * @brief Format the ellipse as a human-readable string.
     * @return String representation of centre and semi-axes
     */
    QString toString() const;

private:
    Point _center;
    double _semiAxisA = 0;
    double _semiAxisB = 0;
};

#endif // ELLIPSE_H
