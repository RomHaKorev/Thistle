#include "linechartlegend.h"
#include "linechart.h"
#include "../base/seriedelegates.h"

namespace Thistle
{
LineChartLegend::LineChartLegend( LineChart* parent ) : AbstractChartLegend( parent ), chart( parent )
{}

LineChartLegend::~LineChartLegend()
{}

/*void LineChartLegend::paint( QPainter & painter ) const {

}*/

void LineChartLegend::paintSerie( QPainter &painter, int serie, QPoint pos, int maxHeight ) const
{
    QPoint p1 = pos + QPoint( 10, - maxHeight/2 );
    QPoint p2 = pos + QPoint( 40, - maxHeight/2 );
    QPoint posText = pos + QPoint( 45, 0 );
    QString s( chart->model()->headerData( serie, Qt::Horizontal ).toString() );
    SerieFormat style = chart->serieFormat( serie );
    Thistle::Types t = style.type();
    painter.drawText( posText, s );
    painter.save();
    QPen pen( style.pen() );
    pen.setWidth( 2 );
    painter.setPen( pen );
    if ( t.testFlag( Thistle::Area ) )
    {
        painter.setBrush( style.brush() );
        QPolygon poly;
        poly << p1 + QPoint( 0, 8 ) << p1 << p1 + QPoint( 10, -8 ) << p1 + QPoint( 15, -3 ) << p1 + QPoint( 20, -6 ) << p2 << p2 + QPoint( 0, 8 );
        painter.drawPolygon( poly );
    }
    else if ( t.testFlag( Thistle::Line ) )
    {
        painter.drawLine( p1, p2 );
    }
    else if ( t.testFlag( Thistle::Spline ) )
    {
        QPainterPath path;
        QPoint p11 = p1 + QPoint( 0, 7 );
        QPoint p22 = p2 - QPoint( 0, 7 );
        path.moveTo( p1 );
        path.cubicTo( p11  + QPoint( 25, 0 ), p22 - QPoint( 25, 0 ), p22 );
        painter.drawPath( path );
    }
    else if ( t.testFlag( Thistle::Bar ) )
    {
        int j = 0;
        Q_FOREACH( int i, QList<int>() << 15 << 8 << 17 << 5 )
        {
            painter.setPen( Qt::NoPen );
            painter.setBrush( style.brush() );
            QRect r( 10 + pos.x() + j * 8, pos.y() - i, 5, i );
            painter.drawRect( r );
            j += 1;
        }
    }

    if ( t.testFlag( Thistle::Dot ) )
    {
        QStyleOptionViewItem option;
        option.rect = QRect( p1.x() + abs(p1.x() - p2.x())/2 - 5, p1.y() - 5, 10, 10 );
        if ( chart->delegate() != 0 ) chart->delegate()->paint( &painter, option, chart->model()->index( 0, serie ) );
    }
    painter.restore();
}
}
