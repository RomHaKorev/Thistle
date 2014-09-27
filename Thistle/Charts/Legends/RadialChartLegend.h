#ifndef RADIALCHARTLEGEND_H
#define RADIALCHARTLEGEND_H
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

#endif // RADIALCHARTLEGEND