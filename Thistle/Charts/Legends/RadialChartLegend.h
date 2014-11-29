#ifndef THISTLE_RADIALCHARTLEGEND_H
#define THISTLE_RADIALCHARTLEGEND_H
#include "chartlegend.h"

namespace Thistle
{

class RadialChart;

class RadialChartLegend : public ChartLegend
{
private:
    RadialChart* chart;
public:
    RadialChartLegend( RadialChart* radialChart );
    ~RadialChartLegend();
protected:
    virtual void paintSerie( QPainter &painter, int serie, QPoint pos, int maxHeight ) const;
};
}

#endif // THISTLE_RADIALCHARTLEGEND
