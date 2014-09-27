#ifndef LINEARLEGENDCHART_H
#define LINEARLEGENDCHART_H

#include <QPainter>
#include <QRect>

#include "chartlegend.h"

namespace Thistle
{
class LinearChart;
class LinearChartLegend : public ChartLegend
{
protected:
    LinearChart* chart;
    virtual void paintSerie( QPainter &painter, int serie, QPoint pos, int maxHeight ) const;
public:
    QRect area;
    LinearChartLegend( LinearChart* parent );
    ~LinearChartLegend();
private:

};
}

#endif // LINEARLEGENDCHART_H
