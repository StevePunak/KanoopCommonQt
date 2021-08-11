#include "geocoordinate.h"
#include <QStringList>
#include "earthgeo.h"

QString GeoCoordinate::TOSTRING_DELIM       = "Lat/Long: ";

bool GeoCoordinate::operator==(const GeoCoordinate &other)
{
    return
        _latitude == other._latitude &&
        _longitude == other._longitude &&
        _altitude == other._altitude &&
        _precision == other._precision &&
        _cardinalLatitude == other._cardinalLatitude &&
            _cardinalLongitude == other._cardinalLongitude;
}

QString GeoCoordinate::toString(Format format, int precision)
{
    QString result;
    switch(format)
    {
    case Parsable:
        result = QString("%1%2, %3")
                .arg(TOSTRING_DELIM)
                .arg(_latitude, precision, 'f')
                .arg(_longitude, precision, 'f');
        break;
    case Degrees:
    default:
        result = QString("%1, %2")
                .arg(_latitude, precision, 'f')
                .arg(_longitude, precision, 'f');
        break;
    }
    return result;
}

bool GeoCoordinate::tryParse(const QString &stringValue, GeoCoordinate& point)
{
    bool result = false;
    QString value = stringValue;
    int		index = value.indexOf(TOSTRING_DELIM);
    QString	name;
    if(index > 0)
    {
        value = value.mid(index).trimmed();
    }

    QStringList parts;
    if(value.length() > TOSTRING_DELIM.length() && value.mid(0, TOSTRING_DELIM.length()) == TOSTRING_DELIM)
    {
        /** parse from our own toString */
        parts = value.mid(TOSTRING_DELIM.length()).split(',');
    }
    else
    {
        /** parse from just lat/long */
        parts = value.split(',');
    }

    if(parts.length() == 2)
    {
        double		latitude, longitude;
        latitude = parts.at(0).toDouble(&result);
        if(result)
        {
            longitude = parts.at(1).toDouble(&result);
            point = GeoCoordinate(latitude, longitude);
            result = true;
        }
    }
    return result;
}

double GeoCoordinate::getDistance(const GeoCoordinate &pt1, const GeoCoordinate &pt2)
{
    // get the arc between the two earth points in radians
    double deltaLat = radians(pt2.latitude() - pt1.latitude());
    double deltaLong = radians(pt2.longitude() - pt1.longitude());

    double latitudeHelix = qPow(qSin(deltaLat * 0.5), 2);
    double longitudeHelix = qPow(qSin(deltaLong * 0.5), 2);

    double tmp = qCos(radians(pt1.latitude())) * qCos(radians(pt2.latitude()));
    double rad = 2.0 * qAsin(qSqrt(latitudeHelix + tmp * longitudeHelix));

    // Multiply the radians by the earth radius in meters for a distance measurement in meters
    return (EarthGeo::EarthRadius * rad);
}
