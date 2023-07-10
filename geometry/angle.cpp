#include "angle.h"

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
    double result = degrees + amount;
    if(result >= 360)
        result -= 360;
    return result;
}

double Angle::subtract(double degrees, double amount)
{
    double result = degrees - amount;
    if(result < 0)
        result += 360;
    return result;
}
