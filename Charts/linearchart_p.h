#ifndef LINEARCHART_P_H
#define LINEARCHART_P_H

#include <QRect>
#include <QString>

#include "axischart_p.h"

namespace Marb {
class PointDelegate;
class BarDelegate;
class OrthogonalAxis;

struct LinearChartPrivate : AxisChartPrivate {
    PointDelegate* pointDelegate;
    BarDelegate* barDelegate;
    OrthogonalAxis* orthoAxis;
    
    LinearChartPrivate();
};

}

#endif // LINEARCHART_P_H
