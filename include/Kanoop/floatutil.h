/**
 * @brief Static floating-point utility helpers.
 */
#ifndef FLOATUTIL_H
#define FLOATUTIL_H

#include "kanoopcommon.h"

/**
 * @brief Static helper methods for floating-point arithmetic checks.
 */
class KANOOP_EXPORT FloatUtil
{
public:
    /**
     * @brief Test whether a dividend is evenly divisible by a divisor.
     * @param dividend Value to divide
     * @param divisor Divisor to use
     * @return true if dividend / divisor produces no remainder
     */
    static bool isEvenlyDivisible(double dividend, double divisor);
};

#endif // FLOATUTIL_H
