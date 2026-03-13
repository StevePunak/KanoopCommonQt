/**
 * @brief A compass bearing angle measured in degrees, with wraparound arithmetic.
 */
#ifndef ANGLE_H
#define ANGLE_H

#include <QString>
#include "Kanoop/kanoopcommon.h"

/**
 * @brief Represents an angle in degrees with wraparound-aware addition and subtraction.
 *
 * Angles are stored in degrees. Arithmetic operators automatically keep values
 * in the range [0, 360).
 */
class KANOOP_EXPORT Angle
{
public:
    /** @brief Default constructor — creates an invalid angle with value -1. */
    Angle() : _degrees(-1) {}

    /**
     * @brief Construct an Angle with the given degree value.
     * @param degrees Angle in degrees
     */
    Angle(double degrees) : _degrees(degrees) {}

    /**
     * @brief Add another Angle, wrapping around 360 degrees.
     * @param other Angle to add
     * @return Resulting angle
     */
    Angle operator+(const Angle& other) const { return *this + other._degrees; }

    /**
     * @brief Add a scalar degree value, wrapping around 360 degrees.
     * @param degrees Degrees to add
     * @return Resulting angle
     */
    Angle operator+(double degrees) const;

    /**
     * @brief Subtract another Angle, wrapping around 360 degrees.
     * @param other Angle to subtract
     * @return Resulting angle
     */
    Angle operator-(const Angle& other) const { return *this - other._degrees; }

    /**
     * @brief Subtract a scalar degree value, wrapping around 360 degrees.
     * @param degrees Degrees to subtract
     * @return Resulting angle
     */
    Angle operator-(double degrees) const;

    /** @brief Add another Angle in place.
     * @param other Angle to add. */
    void operator+=(const Angle& other) { *this = *this + other; }
    /** @brief Add a scalar degree value in place.
     * @param degrees Degrees to add. */
    void operator+=(double degrees) { *this = *this + degrees; }
    /** @brief Subtract another Angle in place.
     * @param other Angle to subtract. */
    void operator-=(const Angle& other) { *this = *this - other; }
    /** @brief Subtract a scalar degree value in place.
     * @param degrees Degrees to subtract. */
    void operator-=(double degrees) { *this = *this - degrees; }

    /**
     * @brief Return the angle in degrees.
     * @return Degree value
     */
    double degrees() const { return _degrees; }

    /**
     * @brief Set the angle in degrees.
     * @param value New degree value
     */
    void setDegrees(double value) { _degrees = value; }

    /**
     * @brief Add a degree amount with wraparound in place.
     * @param degrees Degrees to add
     */
    void add(double degrees);

    /**
     * @brief Subtract a degree amount with wraparound in place.
     * @param degrees Degrees to subtract
     */
    void subtract(double degrees);

    /**
     * @brief Format the angle as a string.
     * @return String of the form "X.Y deg"
     */
    QString toString() const { return QString("%1 deg").arg(_degrees); }

private:
    /** @brief Add an amount to a degree value with wraparound. */
    static double add(double degrees, double amount);
    /** @brief Subtract an amount from a degree value with wraparound. */
    static double subtract(double degrees, double amount);

    double _degrees;
};

#endif // ANGLE_H
