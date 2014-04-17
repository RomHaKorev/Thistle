#ifndef RADIALCHART_P_H
#define RADIALCHART_P_H

#include <QRect>
#include <QString>

#include "axischart_p.h"

namespace Marb {

class RadialAxis;

struct RadialChartPrivate : AxisChartPrivate {
    qreal centerHoleDiam;
    RadialAxis* radialAxis;
    RadialChartPrivate();
};

}

#endif // RADIALCHART_P_H
