/**
 * @brief A 2D triangle defined by three vertices.
 */
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QPointF>
#include "Kanoop/kanoopcommon.h"

/**
 * @brief Represents a 2D triangle by its three vertex points.
 */
class KANOOP_EXPORT Triangle
{
public:
    /** @brief Default constructor — creates a degenerate triangle at the origin. */
    Triangle() {}

    /**
     * @brief Construct a triangle from three vertex points.
     * @param a First vertex
     * @param b Second vertex
     * @param c Third vertex
     */
    Triangle(const QPointF& a, const QPointF& b, const QPointF& c) :
        _a(a), _b(b), _c(c) {}

private:
    const QPointF _a;
    const QPointF _b;
    const QPointF _c;
};

#endif // TRIANGLE_H
