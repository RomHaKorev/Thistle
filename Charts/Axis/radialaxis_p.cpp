#include "radialaxis_p.h"

namespace Thistle {

RadialAxisPrivate::RadialAxisPrivate()
    : AxisPrivate() {
    centerHoleDiam = 0;
    origin = QPointF(0,0);
    nbTicks = 5;
    nbDigits = 0;
}

}