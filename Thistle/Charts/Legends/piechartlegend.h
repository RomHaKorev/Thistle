#ifndef THISTLE_PIECHARTLEGEND_H
#define THISTLE_PIECHARTLEGEND_H

#include "chartlegend.h"

namespace Thistle
{
class PieChart;
class PieChartLegend : public ChartLegend
{
protected:
    PieChart* chart;
    virtual void paint( QPainter& painter ) const;
    virtual void paintSerie( QPainter &painter, int serie, QPoint pos, int maxHeight ) const;
public:
    PieChartLegend( PieChart* parent );
    ~PieChartLegend();
private:

};
}

#endif // THISTLE_PIECHARTLEGEND_H
