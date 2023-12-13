#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QPointF>

class Triangle
{
public:
    Triangle() {}
    Triangle(const QPointF& a, const QPointF& b, const QPointF& c) :
        _a(a), _b(b), _c(c) {}

private:
    const QPointF _a;
    const QPointF _b;
    const QPointF _c;
};

#endif // TRIANGLE_H
