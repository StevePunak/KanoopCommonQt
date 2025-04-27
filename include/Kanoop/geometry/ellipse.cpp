#include "ellipse.h"
#include "flatgeo.h"
#include <Qt>
#include <stringutil.h>

double Ellipse::radiusAtAngle(double angle) const
{
    double s = std::sin(FlatGeo::radians(angle - 90));
    double c = std::cos(FlatGeo::radians(angle - 90));

    double a = _semiAxisA;
    double b = _semiAxisB;
    double result = (a * b) / std::sqrt(((a * a) * (s * s)) + ((b * b) * (c * c)));
    return result;
}

QString Ellipse::toString() const
{
    return QString("c: %1  w: %2  h: %3").arg(_center.toString()).arg(StringUtil::toString(_semiAxisA)).arg(StringUtil::toString(_semiAxisB));
}
