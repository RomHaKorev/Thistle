#ifndef THISTLE_LINEARLEGENDCHART_H
#define THISTLE_LINEARLEGENDCHART_H

#include <QPainter>
#include <QRect>

#include "../base/abstractchartlegend.h"

namespace Thistle
{
class LineChart;
class LineChartLegend : public AbstractChartLegend
{
protected:
    LineChart* chart;
    virtual void paintSerie( QPainter &painter, int serie, QPoint pos, int maxHeight ) const;
public:
    QRect area;
    LineChartLegend( LineChart* parent );
    ~LineChartLegend();
private:

};
}

#endif // THISTLE_LINEARLEGENDCHART_H
