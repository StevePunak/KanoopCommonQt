#ifndef GEOCIRCLE_H
#define GEOCIRCLE_H
#include <QtMath>

#ifdef KANOOP_LIBRARY
#include "Kanoop/geo/geocoordinate.h"
#else
#include <Kanoop/geocoordinate.h>
#endif

#include "Kanoop/kanoopcommon.h"

class KANOOP_EXPORT GeoCircle
{
public:
    GeoCircle() :
        _radius(0) {}

    GeoCircle(const GeoCoordinate& center, double radius) :
        _center(center),
        _radius(radius) {}

    bool operator==(const GeoCircle& other) const;
    bool operator!=(const GeoCircle& other) const { return !(*this == other); }

    GeoCoordinate center() const { return _center; }
    void setCenter(GeoCoordinate value) { _center = value; }

    double radius() const { return _radius; }
    void setRadius(double value) { _radius = value; }

    double diameter() const { return _radius * 2; }
    double area() const { return M_PI * (_radius * _radius); }

    bool contains(const GeoCoordinate& point) const;

    bool isValid() const { return _center.isValid() && _radius != 0; }

    QString toString() const
    {
        return QString("%1, %2").arg(_center.toString()).arg(radius());
    }

    static GeoCircle fromString(const QString& value);

private:
    GeoCoordinate _center;
    double _radius;
};

#endif // GEOCIRCLE_H
