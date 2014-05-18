#ifndef CHARTLEGEND_H
#define CHARTLEGEND_H

#include <QRect>
#include <QPainter>

namespace Thistle {
    class AbstractChart;
    class ChartLegend {
    private:
        AbstractChart* chart;
    public:
        QRect area;
        ChartLegend( AbstractChart* chart );
        ~ChartLegend();
        virtual void paint( QPainter& painter ) const;
    protected:
        virtual void paintSerie( QPainter &painter, int column, QPoint pos, int maxHeight ) const;
    };

}

#endif // CHARTLEGEND_H
