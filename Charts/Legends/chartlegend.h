#ifndef CHARTLEGEND_H
#define CHARTLEGEND_H

#include <QRect>
#include <QPainter>

namespace Thistle {
    class AbstractChart;

    /*!
    \class ChartLegend
    \brief The \a ChartLegend class provides an abstract base for chart legends painting.
    */
    class ChartLegend {
    private:
        AbstractChart* chart;
    public:
        QRect area;
        /*!
        Constructs a \a ChartLegend object for the given \a chart.
        */
        ChartLegend( AbstractChart* chart );
        ~ChartLegend();

        /*!
        Paints the legend of the chart on the given \a QPainter.
        */
        virtual void paint( QPainter& painter ) const;
    protected:

        /*!
        Paints the \a serie on the given \a QPainter. The \a serie will be painted at \a pos.
        \a maxHeight indicates the maximum height allowed to paint the legend. The width should be calculates by the chart itself.
        */
        virtual void paintSerie( QPainter &painter, int serie, QPoint pos, int maxHeight ) const = 0;
    };

}

#endif // CHARTLEGEND_H
