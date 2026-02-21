/**
 * @brief Physical constants and cardinal bearings for Earth-based geodetic calculations.
 */
#ifndef EARTHGEO_H
#define EARTHGEO_H
#include "Kanoop/kanoopcommon.h"

/**
 * @brief Provides physical constants and compass bearing constants for geodetic calculations.
 *
 * All bearings are in degrees, measured clockwise from North (0°).
 */
class KANOOP_EXPORT EarthGeo
{
public:
    EarthGeo();

public:
    static const double EarthRadius;    ///< Mean Earth radius in metres
    static const int    GeoPrecision;   ///< Decimal places used for coordinate comparison
    static const int    LengthPrecision;///< Decimal places used for distance values

    static const double North;      ///< Bearing 0° (due north)
    static const double NorthEast;  ///< Bearing 45°
    static const double East;       ///< Bearing 90°
    static const double SouthEast;  ///< Bearing 135°
    static const double South;      ///< Bearing 180°
    static const double SouthWest;  ///< Bearing 225°
    static const double West;       ///< Bearing 270°
    static const double NorthWest;  ///< Bearing 315°
};

#endif // EARTHGEO_H
