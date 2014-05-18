#ifndef RADIALCHARTLEGEND_H
#define RADIALCHARTLEGEND_H
#include "chartlegend.h"

namespace Thistle {
    class RadialChart;
    class RadialChartLegend : public ChartLegend {
    private:
        RadialChart* chart;
    public:
        RadialChartLegend( RadialChart* radialChart );
        ~RadialChartLegend();
    protected:
        virtual void paintSerie( QPainter &painter, int column, QPoint pos, int maxHeight ) const;
    };
}

#endif // RADIALCHARTLEGEND