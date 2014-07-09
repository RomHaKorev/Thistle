#include "piechartlegend.h"
#include "../piechart.h"

#include <QDebug>

namespace Thistle {
    PieChartLegend::PieChartLegend( PieChart* parent ) : ChartLegend( parent ), chart( parent ) {

    }

    PieChartLegend::~PieChartLegend() {

    }


    void PieChartLegend::paint( QPainter& painter ) const {
        painter.save();
        QFontMetrics metrics( chart->font() );
        int metricsH = metrics.height();
        int h = metricsH + 5;
        int rows = chart->model()->rowCount();
        int w = 0;
        int maxWidth = area.width();
        int y = 0;
        QPoint legendPos( area.topLeft() );
        QPoint pos = legendPos + QPoint( 50, 0);
        for (int r = 0; r < rows; ++r ) {
            QString s( chart->model()->headerData( r, Qt::Horizontal ).toString() );
            QPoint p;
            p = QPoint( area.x(), y + h );
            this->paintSerie(painter, r, p + QPoint(0, metricsH), metricsH);
            y += h;
        }
        painter.restore();
    }

    void PieChartLegend::paintSerie( QPainter &painter, int serie, QPoint pos, int maxHeight ) const {
        QPoint p1 = pos + QPoint( 0, -10 );
        QPoint posText = pos + QPoint( 15, 0 );
        QString s( chart->model()->headerData( serie, Qt::Vertical ).toString() );
        painter.drawText( posText, s );
        painter.save();
        QColor color( chart->model()->data( chart->model()->index( serie, 0 ), Qt::DecorationRole ).toString() );
        if ( !color.isValid() ) {
            color = Colors::predefinedColor( serie );
        }
        chart->configureColor( painter, color );
        painter.drawRect( p1.x(), p1.y(), 10, 10 );
        painter.restore();
    }
}
