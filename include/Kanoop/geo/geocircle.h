/**
 * @brief A geographic circle defined by a GeoCoordinate centre and a radius in metres.
 */
#ifndef GEOCIRCLE_H
#define GEOCIRCLE_H
#include <QtMath>

#include "Kanoop/geo/geocoordinate.h"
#include "Kanoop/kanoopcommon.h"

/**
 * @brief Represents a circular region on the Earth's surface.
 *
 * The centre is a GeoCoordinate and the radius is in metres.
 * Use contains() to test whether another coordinate falls within the circle.
 */
class KANOOP_EXPORT GeoCircle
{
public:
    /** @brief Default constructor — creates an invalid circle with zero radius. */
    GeoCircle() :
        _radius(0) {}

    /**
     * @brief Construct a GeoCircle from a centre coordinate and radius.
     * @param center Centre of the circle (geographic coordinate)
     * @param radius Radius in metres
     */
    GeoCircle(const GeoCoordinate& center, double radius) :
        _center(center),
        _radius(radius) {}

    /** @brief Equality comparison. */
    bool operator==(const GeoCircle& other) const;
    /** @brief Inequality comparison. */
    bool operator!=(const GeoCircle& other) const { return !(*this == other); }

    /**
     * @brief Return the centre coordinate.
     * @return Centre GeoCoordinate
     */
    GeoCoordinate center() const { return _center; }

    /**
     * @brief Set the centre coordinate.
     * @param value New centre GeoCoordinate
     */
    void setCenter(GeoCoordinate value) { _center = value; }

    /**
     * @brief Return the radius in metres.
     * @return Radius value
     */
    double radius() const { return _radius; }

    /**
     * @brief Set the radius in metres.
     * @param value New radius value
     */
    void setRadius(double value) { _radius = value; }

    /**
     * @brief Return the diameter in metres.
     * @return Diameter (2 × radius)
     */
    double diameter() const { return _radius * 2; }

    /**
     * @brief Return the area in square metres.
     * @return Area (π × radius²)
     */
    double area() const { return M_PI * (_radius * _radius); }

    /**
     * @brief Test whether a coordinate falls within this circle.
     * @param point Coordinate to test
     * @return true if the great-circle distance from centre to point is ≤ radius
     */
    bool contains(const GeoCoordinate& point) const;

    /**
     * @brief Test whether this circle has a valid centre and non-zero radius.
     * @return true if centre is valid and radius != 0
     */
    bool isValid() const { return _center.isValid() && _radius != 0; }

    /**
     * @brief Format this circle as a human-readable string.
     * @return String of the form "lat, lon, radius"
     */
    QString toString() const
    {
        return QString("%1, %2").arg(_center.toString()).arg(radius());
    }

    /**
     * @brief Parse a GeoCircle from a string produced by toString().
     * @param value String representation
     * @return Parsed GeoCircle
     */
    static GeoCircle fromString(const QString& value);

private:
    GeoCoordinate _center;
    double _radius;
};

#endif // GEOCIRCLE_H
