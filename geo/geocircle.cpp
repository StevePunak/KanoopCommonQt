#include "geocircle.h"


bool GeoCircle::contains(const GeoCoordinate &point) const
{
    double distance = _center.distanceTo(point);
    return distance <= _radius;
}
