#ifndef RADIALCHART_P_H
#define RADIALCHART_P_H

#include <QRect>
#include <QString>

#include "axischart_p.h"

namespace Thistle
{

class RadialAxis;
class RadialChart;
class AxisChart;

struct RadialChartPrivate : public AxisChartPrivate
{
    qreal centerHoleDiam;
    RadialAxis* radialAxis;
    RadialChartPrivate( RadialChart* q );
    ~RadialChartPrivate();
};

}

#endif // RADIALCHART_P_H
