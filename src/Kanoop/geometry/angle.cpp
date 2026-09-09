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

void Angle::add(double degrees)
{
    _degrees = add(_degrees, degrees);
}

void Angle::subtract(double degrees)
{
    _degrees = subtract(_degrees, degrees);
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
