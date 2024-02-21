#ifndef SIZE_H
#define SIZE_H
#include <QSizeF>
#include "Kanoop/kanoopcommon.h"

class Rectangle;
class KANOOP_EXPORT Size : public QSizeF
{
public:
    Size() : QSizeF() {}
    Size(const QSizeF& other) : QSizeF(other) {}
    Size(const QSize& other) : QSizeF(other) {}
    Size(double w, double h) : QSizeF(w, h) {}

    Rectangle toRectangle() const;

    QString toString() const { return QString("%1, %2").arg(width()).arg(height()); }
    static Size fromString(const QString& value);
};

#endif // SIZE_H
