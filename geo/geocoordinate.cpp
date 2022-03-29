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

bool GeoCoordinate::isNorthOf(const GeoCoordinate &other)
{
    bool result = false;
    if(_cardinalLatitude == Geo::North && other._cardinalLatitude == Geo::North)
        result = _latitude > other._latitude;
    else if(_cardinalLatitude == Geo::South && other._cardinalLatitude == Geo::South)
        result = _latitude < other._latitude;
    else
        result = _cardinalLatitude == Geo::North;
    return result;
}

bool GeoCoordinate::isWestOf(const GeoCoordinate &other)
{
    bool result = false;
    if(_cardinalLongitude == Geo::West && other._cardinalLongitude == Geo::West)
        result = _longitude < other._longitude;
    else if(_cardinalLongitude == Geo::East && other._cardinalLongitude == Geo::East)
        result = _longitude > other._longitude;
    else
        result = _cardinalLongitude == Geo::West;
    return result;
}

QString GeoCoordinate::toString(CoordinateFormat format, int precision) const
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
    double deltaLat = qDegreesToRadians(pt2.latitude() - pt1.latitude());
    double deltaLong = qDegreesToRadians(pt2.longitude() - pt1.longitude());

    double latitudeHelix = qPow(qSin(deltaLat * 0.5), 2);
    double longitudeHelix = qPow(qSin(deltaLong * 0.5), 2);

    double tmp = qCos(qDegreesToRadians(pt1.latitude())) * qCos(qDegreesToRadians(pt2.latitude()));
    double rad = 2.0 * qAsin(qSqrt(latitudeHelix + tmp * longitudeHelix));

    // Multiply the radians by the earth radius in meters for a distance measurement in meters
    return (EarthGeo::EarthRadius * rad);
}

double GeoCoordinate::getAzimuth(const GeoCoordinate &pt1, const GeoCoordinate &pt2)
{
    double lat1 = qDegreesToRadians(pt1.latitude());
    double lon1 = qDegreesToRadians(pt1.longitude());
    double lat2 = qDegreesToRadians(pt2.latitude());
    double lon2 = qDegreesToRadians(pt2.longitude());

    double y = qSin(lon2 - lon1) * qCos(lat2);
    double x = qCos(lat1) * qSin(lat2) - qSin(lat1) * qCos(lat2) * qCos(lon2 - lon1);
    double bearing = qAtan2(y, x); 	// bearing in radians

    double ret = fmod(qRadiansToDegrees(bearing) + 360, 360);
    return ret;
}

GeoCoordinate GeoCoordinate::getPointAtDistanceAndAzimuth(const GeoCoordinate& origin, double distance, double azimuth)
{
    int divs = azimuth / 360;       // TODO consolidate math
    if(divs > 0)
        azimuth -= divs * 360;
    azimuth = qDegreesToRadians(fmod(azimuth, 360));
    double ddr = distance / EarthGeo::EarthRadius;

    double lat = qDegreesToRadians(origin.latitude());
    double lon = qDegreesToRadians(origin.longitude());


    double lat2 = qAsin(qSin(lat) * qCos(ddr) + qCos(lat) * qSin(ddr) * qCos(azimuth));
    double lon2 = lon + qAtan2(qSin(azimuth) * qSin(ddr) * qCos(lat), qCos(ddr) - qSin(lat) * qSin(lat2));
    lon2 = fmod((lon2 + M_PI), (2 * M_PI)) - M_PI;

    return GeoCoordinate(qRadiansToDegrees(lat2), qRadiansToDegrees(lon2));
}

bool GeoCoordinate::equalAtPrecision(double v1, double v2, int precision)
{
    QString s1 = QString::number(v1, 'f', precision);
    QString s2 = QString::number(v2, 'f', precision);
    return s1 == s2;
}

