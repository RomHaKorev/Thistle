#ifndef PIECHARTLEGEND_H
#define PIECHARTLEGEND_H

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

#endif // PIECHARTLEGEND_H
