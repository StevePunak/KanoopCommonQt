#ifndef EARTHGEO_H
#define EARTHGEO_H

class EarthGeo
{
public:
    EarthGeo();

public:
    static const double EarthRadius;
    static const int	GeoPrecision;
    static const int	LengthPrecision;

    static const double North;
    static const double NorthEast;
    static const double East;
    static const double SouthEast;
    static const double South;
    static const double SouthWest;
    static const double West;
    static const double NorthWest;
};

#endif // EARTHGEO_H
