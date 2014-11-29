#ifndef THISTLE_XYCHART_P_H
#define THISTLE_XYCHART_P_H

#include "linearchart_p.h"
#include "Axis/abstractcoordinatesystem.h"

#include <QMap>

namespace Thistle
{
class XYChart;
struct XYChartPrivate : public LinearChartPrivate
{
    XYChartPrivate( XYChart* q );
};

}

#endif // THISTLE_XYCHART_P_H
