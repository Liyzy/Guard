#ifndef COLLIDE_H
#define COLLIDE_H
#include <QPoint>
#include <QtMath>

inline bool collideWithCircle(const QPoint point1, int radius1, const QPoint point2, int radius2)
{
    const int xdif = point1.x()-point2.x();
    const int ydif = point1.y()-point2.y();
    const double distance = qSqrt(xdif * xdif + ydif * ydif);
    if(distance <= radius1 + radius2)
        return true;
    return false;
}

#endif // COLLIDE_H
