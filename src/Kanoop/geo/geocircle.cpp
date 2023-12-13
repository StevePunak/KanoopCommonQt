#include "Kanoop/geo/geocircle.h"
#include <QStringList>

bool GeoCircle::operator==(const GeoCircle &other) const
{
    bool result =
        _center == other._center &&
        _radius == other._radius;
    return result;
}

bool GeoCircle::contains(const GeoCoordinate &point) const
{
    double distance = _center.distanceTo(point);
    return distance <= _radius;
}

GeoCircle GeoCircle::fromString(const QString &value)
{
    GeoCircle result;
    QStringList parts = value.split(',');
    for(int i = 0;i < parts.count();i++) {
        QString part = parts.at(i).trimmed();
        if(i == 0) {
            result._center.setLatitude(part.toDouble());
        }
        else if(i == 1) {
            result._center.setLongitude(part.toDouble());
        }
        else if(i == 2) {
            result._radius = part.toDouble();
        }
    }
    return result;
}
