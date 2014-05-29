#ifndef RADIALAXIS_P_H
#define RADIALAXIS_P_H

#include "abstractaxis_p.h"

namespace Thistle {

    struct RadialAxisPrivate : public AbstractAxisPrivate {
        qreal centerHoleDiam;
        QPointF origin;
        QLineF yaxis;

        RadialAxisPrivate();
    };

}

#endif // RADIALAXIS_P_H
