#ifndef ELLIPSE_H
#define ELLIPSE_H
#include <Kanoop/geometry/point.h>

class KANOOP_EXPORT Ellipse
{
public:
    Ellipse() {}
    Ellipse(const Point& center, double semiAxisA, double semiAxisB) :
        _center(center), _semiAxisA(semiAxisA), _semiAxisB(semiAxisB) {}

    Point center() const { return _center; }
    void setCenter(const Point& value) { _center = value; }

    double semiAxisA() const { return  _semiAxisA; }
    void setSemiAxisA(double value) { _semiAxisA = value; }

    double semiAxisB() const { return  _semiAxisB; }
    void setSemiAxisB(double value) { _semiAxisB = value; }

    /**
     * @brief radiusAtAngle
     * @param angle in degrees offset from vertical (north) at zero
     * @return The radius at the given angle.
     */
    double radiusAtAngle(double angle) const;

    QString toString() const;

private:
    Point _center;
    double _semiAxisA = 0;
    double _semiAxisB = 0;
};

#endif // ELLIPSE_H
