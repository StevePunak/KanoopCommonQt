#include "Kanoop/geo/earthgeo.h"

const double EarthGeo::EarthRadius              = 6372797.560856;
const int	EarthGeo::GeoPrecision              = 8;
const int	EarthGeo::LengthPrecision           = 3;		/** one millimeter */

const double EarthGeo::North                    = 0;
const double EarthGeo::NorthEast                = 45;
const double EarthGeo::East                     = 90;
const double EarthGeo::SouthEast                = 135;
const double EarthGeo::South                    = 180;
const double EarthGeo::SouthWest                = 225;
const double EarthGeo::West                     = 270;
const double EarthGeo::NorthWest                = 315;

EarthGeo::EarthGeo()
{

}
