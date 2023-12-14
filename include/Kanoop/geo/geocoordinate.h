#ifndef GEOCOORDINATE_H
#define GEOCOORDINATE_H

#ifdef KANOOP_LIBRARY
#include "Kanoop/geo/geotypes.h"
#else
#include <Kanoop/geotypes.h>
#endif
#include <QString>
#include <QtMath>

#include "Kanoop/kanoopcommon.h"

class KANOOP_EXPORT GeoCoordinate
{
public:
    GeoCoordinate() :
        _latitude(0),
        _longitude(0),
        _altitude(0),
        _precision(6),
        _cardinalLatitude(Geo::InvalidCardinalDirection),
        _cardinalLongitude(Geo::InvalidCardinalDirection),
        _valid(false) {}

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

    GeoCoordinate(const QString& value);

    static GeoCoordinate fromString(const QString& value)
    {
        return GeoCoordinate(value);
    }

    static GeoCoordinate fromDMS(Geo::CardinalDirection latitudeCardinalDirection, double latitudeDegrees, double latitudeMinutes, double latitudeSeconds,
                                 Geo::CardinalDirection longitudeCardinalDirection, double longitudeDegrees, double longitudeMinutes, double longitudeSeconds);


    enum CoordinateFormat
    {
        Degrees,
        Parsable,
    };

    bool operator==(const GeoCoordinate& other) const;
    bool operator!=(const GeoCoordinate& other) const { return !(*this == other); }

    double latitude() const { return _latitude; }
    void setLatitude(double value) { _latitude = value; validate(); }

    double longitude() const { return _longitude; }
    void setLongitude(double value) { _longitude = value; validate(); }

    double altitude() const { return _altitude; }
    void setAltitude(double value) { _altitude = value; validate(); }

    int precision() const { return _precision; }
    void setPrecision(int value) { _precision = value; }

    double distanceTo(const GeoCoordinate& other)  const { return getDistance(*this, other); }
    double azimuthTo(const GeoCoordinate& other)  const { return getAzimuth(*this, other); }
    GeoCoordinate atDistanceAndAzimuth(double distance, double azimuth) { return getPointAtDistanceAndAzimuth(*this, distance, azimuth); }

    bool isNorthOfOrEqualTo(const GeoCoordinate& other) const
    {
        return isNorthOf(other) || (equalAtPrecision(_latitude, other._latitude, _precision) && _cardinalLatitude == other._cardinalLatitude);
    }
    bool isNorthOf(const GeoCoordinate& other) const;
    bool isSouthOfOrEqualTo(const GeoCoordinate& other) const
    {
        return isSouthOf(other) || (equalAtPrecision(_latitude, other._cardinalLatitude, _precision) && _cardinalLatitude == other._cardinalLatitude);
    }
    bool isSouthOf(const GeoCoordinate& other)  const{ return !isNorthOf(other); }
    bool isWestOfOrEqualTo(const GeoCoordinate& other) const
    {
        return isWestOf(other) || equalAtPrecision(_longitude, other._longitude, _precision);
    }
    bool isWestOf(const GeoCoordinate& other) const;
    bool isEastOfOrEqualTo(const GeoCoordinate& other) const
    {
        return isEastOf(other) || equalAtPrecision(_longitude, other._longitude, _precision);
    }
    bool isEastOf(const GeoCoordinate& other) const { return !isWestOf(other); }

    bool isEmpty() const { return *this == GeoCoordinate(); }
    bool isValid() const { return !isEmpty(); }

    QString toString(CoordinateFormat format = Degrees, int precision = 0) const;

    static bool tryParse(const QString& stringValue, GeoCoordinate &point);
    static GeoCoordinate empty() { return GeoCoordinate(); }

    static double getDistance(const GeoCoordinate& pt1, const GeoCoordinate& pt2);
    static double getAzimuth(const GeoCoordinate& pt1, const GeoCoordinate& pt2);
    static GeoCoordinate getPointAtDistanceAndAzimuth(const GeoCoordinate& origin, double distance, double azimuth);

private:
    void validate();

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
