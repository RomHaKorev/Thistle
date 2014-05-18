#ifndef RADIALAXIS_P_H
#define RADIALAXIS_P_H

#include "axis_p.h"

namespace Thistle {

struct RadialAxisPrivate : public AxisPrivate {
    qreal centerHoleDiam;
    QPointF origin;
    QLineF yaxis;

    RadialAxisPrivate();
};

}

#endif // RADIALAXIS_P_H
