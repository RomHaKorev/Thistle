#ifndef AXISCHART_P_H
#define AXISCHART_P_H

#include "abstractchart_p.h"
#include "Axis/axis.h"
#include "chartstyle.h"

namespace Marb {

struct AxisChartPrivate : AbstractChartPrivate {
    Axis* axis;
    QMap<int, ChartStyle> style;
    int x;

    AxisChartPrivate();
};

}

#endif // AXISCHART_P_H
