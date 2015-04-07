#include "chartlegend.h"

#include "../kernel/abstractitemview.h"

namespace Thistle
{
ChartLegend::ChartLegend( AbstractItemView* parent ) : chart( parent )
{}

ChartLegend::~ChartLegend()
{}

void ChartLegend::paint( QPainter& painter ) const
{
    if ( chart->model() == 0 )
        return;

    painter.save();
    QFontMetrics metrics( chart->font() );
    int metricsH = metrics.height();
    int h = metricsH + 5;
    int cols = chart->model()->columnCount();
    int w = 0;
    int maxWidth = area.width();
    QPoint legendPos( area.topLeft() );
    QPoint pos = legendPos + QPoint( 50, 0);
    for (int c = 0; c < cols; ++c )
    {
        QString s( chart->model()->headerData( c, Qt::Horizontal ).toString() );
        int sWidth = metrics.width( s ) + 50;
        QPoint p;
        if ( ( w + sWidth ) > maxWidth )
        {
            int y = pos.y();
            p = QPoint( area.x() + 10, y + h );
            pos = QPoint( area.x() + 50, y + h );
            w = sWidth;
            pos += QPoint( sWidth, 0 );
        }
        else
        {
            p = pos + QPoint( -40,    0 );
            w += sWidth;
            pos += QPoint( sWidth, 0 );
        }
        this->paintSerie(painter, c, p + QPoint(0, metricsH), metricsH);
    }
    painter.restore();
}

void ChartLegend::paintSerie( QPainter &painter, int serie, QPoint pos, int maxHeight ) const
{
  Q_UNUSED( maxHeight)
    QPoint posText = pos + QPoint( 45, 0 );
    QString s( chart->model()->headerData( serie, Qt::Horizontal ).toString() );
    painter.drawText( posText, s );
}

}
