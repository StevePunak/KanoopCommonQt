/**
 * @brief A WGS-84 geographic coordinate (latitude, longitude, altitude).
 */
#ifndef GEOCOORDINATE_H
#define GEOCOORDINATE_H

#include "Kanoop/geo/geotypes.h"
#include <QString>
#include <QtMath>

#include "Kanoop/kanoopcommon.h"

/**
 * @brief Represents a geographic position as WGS-84 latitude, longitude, and altitude.
 *
 * Provides distance, azimuth, and point-at-distance-and-azimuth calculations
 * using the Haversine formula, as well as cardinal direction comparisons.
 */
class KANOOP_EXPORT GeoCoordinate
{
public:
    /** @brief Default constructor — creates an invalid coordinate at (0, 0, 0). */
    GeoCoordinate() :
        _latitude(0),
        _longitude(0),
        _altitude(0),
        _precision(6),
        _cardinalLatitude(Geo::InvalidCardinalDirection),
        _cardinalLongitude(Geo::InvalidCardinalDirection),
        _valid(false) {}

    /**
     * @brief Construct a coordinate from decimal latitude, longitude, and optional altitude.
     * @param latitude Latitude in decimal degrees (negative = south)
     * @param longitude Longitude in decimal degrees (negative = west)
     * @param altitude Altitude in metres above the WGS-84 ellipsoid (default 0)
     */
    GeoCoordinate(double latitude, double longitude, double altitude = 0) :
        _latitude(latitude),
        _longitude(longitude),
        _altitude(altitude),
        _precision(6),
        _cardinalLatitude(Geo::InvalidCardinalDirection),
        _cardinalLongitude(Geo::InvalidCardinalDirection),
        _valid(false)
    {
        GeoCoordinate::validate();
    }

    /**
     * @brief Construct a coordinate by parsing a string.
     * @param value Coordinate string (see fromString())
     */
    GeoCoordinate(const QString& value);

    /**
     * @brief Parse a GeoCoordinate from a string representation.
     * @param value Coordinate string in decimal or DMS format
     * @return Parsed GeoCoordinate, or an invalid one on failure
     */
    static GeoCoordinate fromString(const QString& value)
    {
        return GeoCoordinate(value);
    }

    /**
     * @brief Construct a coordinate from degrees-minutes-seconds components.
     * @param latitudeCardinalDirection North or South
     * @param latitudeDegrees Whole degrees of latitude
     * @param latitudeMinutes Minutes of latitude
     * @param latitudeSeconds Seconds of latitude
     * @param longitudeCardinalDirection East or West
     * @param longitudeDegrees Whole degrees of longitude
     * @param longitudeMinutes Minutes of longitude
     * @param longitudeSeconds Seconds of longitude
     * @return Constructed GeoCoordinate
     */
    static GeoCoordinate fromDMS(Geo::CardinalDirection latitudeCardinalDirection, double latitudeDegrees, double latitudeMinutes, double latitudeSeconds,
                                 Geo::CardinalDirection longitudeCardinalDirection, double longitudeDegrees, double longitudeMinutes, double longitudeSeconds);

    /**
     * @brief Output format for toString().
     */
    enum CoordinateFormat
    {
        Degrees,    ///< Decimal degrees (e.g. "37.7749, -122.4194")
        Parsable,   ///< Format suitable for round-trip parsing by fromString()
    };

    /** @brief Equality comparison at the configured precision. */
    bool operator==(const GeoCoordinate& other) const;
    /** @brief Inequality comparison. */
    bool operator!=(const GeoCoordinate& other) const { return !(*this == other); }

    /**
     * @brief Return the latitude in decimal degrees.
     * @return Latitude (negative = south)
     */
    double latitude() const { return _latitude; }

    /**
     * @brief Set the latitude in decimal degrees.
     * @param value New latitude value
     */
    void setLatitude(double value) { _latitude = value; validate(); }

    /**
     * @brief Return the longitude in decimal degrees.
     * @return Longitude (negative = west)
     */
    double longitude() const { return _longitude; }

    /**
     * @brief Set the longitude in decimal degrees.
     * @param value New longitude value
     */
    void setLongitude(double value) { _longitude = value; validate(); }

    /**
     * @brief Return the altitude in metres.
     * @return Altitude above the WGS-84 ellipsoid
     */
    double altitude() const { return _altitude; }

    /**
     * @brief Set the altitude in metres.
     * @param value New altitude value
     */
    void setAltitude(double value) { _altitude = value; validate(); }

    /**
     * @brief Return the decimal precision used for coordinate comparisons.
     * @return Number of decimal places
     */
    int precision() const { return _precision; }

    /**
     * @brief Set the decimal precision used for coordinate comparisons.
     * @param value Number of decimal places
     */
    void setPrecision(int value) { _precision = value; }

    /**
     * @brief Compute the great-circle distance to another coordinate in metres.
     * @param other Destination coordinate
     * @return Distance in metres
     */
    double distanceTo(const GeoCoordinate& other)  const { return getDistance(*this, other); }

    /**
     * @brief Compute the azimuth (bearing) to another coordinate in degrees.
     * @param other Destination coordinate
     * @return Azimuth in degrees clockwise from North
     */
    double azimuthTo(const GeoCoordinate& other)  const { return getAzimuth(*this, other); }

    /**
     * @brief Return the coordinate reached by travelling a given distance and azimuth from this point.
     * @param distance Distance in metres
     * @param azimuth Bearing in degrees clockwise from North
     * @return Resulting GeoCoordinate
     */
    GeoCoordinate atDistanceAndAzimuth(double distance, double azimuth) { return getPointAtDistanceAndAzimuth(*this, distance, azimuth); }

    /**
     * @brief Test whether this coordinate is north of or at the same latitude as another.
     * @param other Coordinate to compare against
     * @return true if this latitude ≥ other's latitude (at configured precision)
     */
    bool isNorthOfOrEqualTo(const GeoCoordinate& other) const
    {
        return isNorthOf(other) || (equalAtPrecision(_latitude, other._latitude, _precision) && _cardinalLatitude == other._cardinalLatitude);
    }

    /**
     * @brief Test whether this coordinate is strictly north of another.
     * @param other Coordinate to compare against
     * @return true if this latitude > other's latitude
     */
    bool isNorthOf(const GeoCoordinate& other) const;

    /**
     * @brief Test whether this coordinate is south of or at the same latitude as another.
     * @param other Coordinate to compare against
     * @return true if this latitude ≤ other's latitude (at configured precision)
     */
    bool isSouthOfOrEqualTo(const GeoCoordinate& other) const
    {
        return isSouthOf(other) || (equalAtPrecision(_latitude, other._cardinalLatitude, _precision) && _cardinalLatitude == other._cardinalLatitude);
    }

    /**
     * @brief Test whether this coordinate is strictly south of another.
     * @param other Coordinate to compare against
     * @return true if this latitude < other's latitude
     */
    bool isSouthOf(const GeoCoordinate& other)  const{ return !isNorthOf(other); }

    /**
     * @brief Test whether this coordinate is west of or at the same longitude as another.
     * @param other Coordinate to compare against
     * @return true if this longitude ≤ other's longitude (at configured precision)
     */
    bool isWestOfOrEqualTo(const GeoCoordinate& other) const
    {
        return isWestOf(other) || equalAtPrecision(_longitude, other._longitude, _precision);
    }

    /**
     * @brief Test whether this coordinate is strictly west of another.
     * @param other Coordinate to compare against
     * @return true if this longitude < other's longitude
     */
    bool isWestOf(const GeoCoordinate& other) const;

    /**
     * @brief Test whether this coordinate is east of or at the same longitude as another.
     * @param other Coordinate to compare against
     * @return true if this longitude ≥ other's longitude (at configured precision)
     */
    bool isEastOfOrEqualTo(const GeoCoordinate& other) const
    {
        return isEastOf(other) || equalAtPrecision(_longitude, other._longitude, _precision);
    }

    /**
     * @brief Test whether this coordinate is strictly east of another.
     * @param other Coordinate to compare against
     * @return true if this longitude > other's longitude
     */
    bool isEastOf(const GeoCoordinate& other) const { return !isWestOf(other); }

    /**
     * @brief Test whether this coordinate equals the default-constructed (empty) value.
     * @return true if this coordinate is at (0, 0, 0) with no cardinal directions set
     */
    bool isEmpty() const { return *this == GeoCoordinate(); }

    /**
     * @brief Test whether this coordinate represents a valid position.
     * @return true if the coordinate is not empty
     */
    bool isValid() const { return !isEmpty(); }

    /**
     * @brief Format this coordinate as a string.
     * @param format Output format (Degrees or Parsable)
     * @param precision Decimal places for coordinate values (0 = use instance precision)
     * @return Formatted coordinate string
     */
    QString toString(CoordinateFormat format = Degrees, int precision = 0) const;

    /**
     * @brief Attempt to parse a coordinate string, reporting success.
     * @param stringValue String to parse
     * @param point Output receiving the parsed coordinate
     * @return true if parsing succeeded
     */
    static bool tryParse(const QString& stringValue, GeoCoordinate &point);

    /**
     * @brief Return a default-constructed (empty) coordinate.
     * @return Empty GeoCoordinate
     */
    static GeoCoordinate empty() { return GeoCoordinate(); }

    /**
     * @brief Compute the great-circle distance between two coordinates in metres.
     * @param pt1 First coordinate
     * @param pt2 Second coordinate
     * @return Distance in metres
     */
    static double getDistance(const GeoCoordinate& pt1, const GeoCoordinate& pt2);

    /**
     * @brief Compute the azimuth from one coordinate to another in degrees.
     * @param pt1 Origin coordinate
     * @param pt2 Destination coordinate
     * @return Azimuth in degrees clockwise from North
     */
    static double getAzimuth(const GeoCoordinate& pt1, const GeoCoordinate& pt2);

    /**
     * @brief Compute the coordinate reached by travelling from an origin at a given distance and azimuth.
     * @param origin Starting coordinate
     * @param distance Distance in metres
     * @param azimuth Bearing in degrees clockwise from North
     * @return Resulting GeoCoordinate
     */
    static GeoCoordinate getPointAtDistanceAndAzimuth(const GeoCoordinate& origin, double distance, double azimuth);

private:
    /** @brief Validate and set the _valid flag based on the current coordinate values. */
    void validate();

    /** @brief Test two doubles for equality at a given number of decimal places. */
    static bool equalAtPrecision(double v1, double v2, int precision);

    double _latitude;
    double _longitude;
    double _altitude;

    int _precision;

    Geo::CardinalDirection _cardinalLatitude;
    Geo::CardinalDirection _cardinalLongitude;

    bool _valid;

    static QString TOSTRING_DELIM;
};

#endif // GEOCOORDINATE_H
