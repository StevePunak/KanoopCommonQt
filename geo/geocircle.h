#ifndef GEOCIRCLE_H
#define GEOCIRCLE_H
#include "geocoordinate.h"
#include <QtMath>
#include "kanoopcommon.h"

class KANOOP_EXPORT GeoCircle
{
public:
    GeoCircle() :
        _radius(0) {}

    GeoCircle(const GeoCoordinate& center, double radius) :
        _center(center),
        _radius(radius) {}

    GeoCoordinate center() const { return _center; }
    void setCenter(GeoCoordinate value) { _center = value; }

    double radius() const { return _radius; }
    void setRadius(double value) { _radius = value; }

    double diameter() const { return _radius * 2; }
    double area() const { return M_PI * (_radius * _radius); }

    bool contains(const GeoCoordinate& point);

private:
    GeoCoordinate _center;
    double _radius;
};

#endif // GEOCIRCLE_H
