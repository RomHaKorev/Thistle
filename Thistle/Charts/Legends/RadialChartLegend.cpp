#include "RadialChartLegend.h"
#include <QPainter>
#include <QPoint>

#include "../radialchart.h"

namespace Thistle {
    RadialChartLegend::RadialChartLegend( RadialChart* radialChart ) : ChartLegend( radialChart ), chart ( radialChart ) {
    }


    RadialChartLegend::~RadialChartLegend()     {
    }

    void RadialChartLegend::paintSerie( QPainter &painter, int serie, QPoint pos, int maxHeight ) const {
        QRect r( pos.x() + 25, pos.y() - 15, 30, 30 );
        QPoint posText = pos + QPoint( 45, 0 );
        QString s( chart->model()->headerData( serie, Qt::Horizontal ).toString() );
        SerieFormat style = chart->serieFormat( serie );
        Thistle::Types t = style.type();
        painter.drawText( posText, s );
        painter.save();
        painter.setPen( style.pen() );
        painter.setBrush( style.brush() );
        painter.drawPie( r, 210 * 16, -60 * 16 );
        painter.restore();
    }
}