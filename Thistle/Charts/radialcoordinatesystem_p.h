#ifndef THISTLE_RADIALAXIS_P_H
#define THISTLE_RADIALAXIS_P_H

#include "abstractcoordinatesystem_p.h"

namespace Thistle
{

struct RadialCoordinateSystemPrivate : public AbstractCoordinateSystemPrivate
{
    qreal centerHoleDiam;
    QPointF origin;
    QLineF yaxis;

    RadialCoordinateSystemPrivate();
};

}

#endif // THISTLE_RADIALAXIS_P_H
