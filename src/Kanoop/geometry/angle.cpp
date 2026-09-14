#include <QtMath>

#include "Kanoop/geometry/angle.h"

Angle Angle::operator+(double degrees) const
{
    double result = add(_degrees, degrees);
    return Angle(result);
}

Angle Angle::operator-(double degrees) const
{
    double result = subtract(_degrees, degrees);
    return Angle(result);
}

double Angle::add(double degrees, double amount)
{
    return normalize(degrees + amount);
}

double Angle::subtract(double degrees, double amount)
{
    return normalize(degrees - amount);
}

double Angle::normalize(double degrees)
{
    return std::fmod(std::fmod(degrees, 360.0) + 360.0, 360.0);
}
