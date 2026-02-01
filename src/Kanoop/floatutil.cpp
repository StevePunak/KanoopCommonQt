#include "floatutil.h"
#include <math.h>

bool FloatUtil::isEvenlyDivisible(double dividend, double divisor)
{
    if(divisor == 0) {
        return false;
    }

    const double EPSILON = 1e-9;

    // Calculate the remainder
    double remainder = std::fmod(dividend, divisor);

    // Check if the remainder is close to zero
    if (std::abs(remainder) < EPSILON || std::abs(remainder - divisor) < EPSILON) {
        return true;
    }
    return false;
}
