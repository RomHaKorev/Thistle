#include "linearchartlegend.h"
#include "../linearchart.h"
#include "delegates.h"

namespace Thistle {
    LinearChartLegend::LinearChartLegend( LinearChart* parent ) : ChartLegend( parent ), chart( parent ) {

    }

    LinearChartLegend::~LinearChartLegend() {

    }

    /*void LinearChartLegend::paint( QPainter & painter ) const {

    }*/

    void LinearChartLegend::paintSerie( QPainter &painter, int column, QPoint pos, int maxHeight ) const {
        QPoint p1 = pos + QPoint( 10, - maxHeight/2 );
        QPoint p2 = pos + QPoint( 40, - maxHeight/2 );
        QPoint posText = pos + QPoint( 45, 0 );
        QString s( chart->model()->headerData( column, Qt::Horizontal ).toString() );
        SerieFormat style = chart->serieFormat( column );
        Global::Types t = style.type();
        painter.drawText( posText, s );
        painter.save();
        painter.setPen( style.pen() );
        if ( t.testFlag( Global::Area ) ) {
            painter.setBrush( style.brush() );
            QPolygon poly;
            poly << p1 + QPoint( 0, 8 ) << p1 << p1 + QPoint( 10, -8 ) << p1 + QPoint( 15, -3 ) << p1 + QPoint( 20, -6 ) << p2 << p2 + QPoint( 0, 8 );
            painter.drawPolygon( poly );
        } else if ( t.testFlag( Global::Line ) ) {
            painter.drawLine( p1, p2 );
        } else if ( t.testFlag( Global::Spline ) ) {
            QPainterPath path;
            p1 += QPoint( 0, 7 );
            p2 -= QPoint( 0, 7 );
            path.moveTo( p1 );
            path.cubicTo( p1  + QPoint( 25, 0 ), p2 - QPoint( 25, 0 ), p2 );
            painter.drawPath( path );
        } else if ( t.testFlag( Global::Bar ) ) {
            int j = 0;
            Q_FOREACH( int i, QList<int>() << 15 << 8 << 17 << 5 ) {
                painter.setPen( Qt::NoPen );
                painter.setBrush( style.brush() );
                QRect r( 10 + pos.x() + j * 8, pos.y() - i, 5, i );
                painter.drawRect( r );
                j += 1;
            }
        }
        
        if ( t.testFlag( Global::Dot ) ) {
            QStyleOptionViewItem option;
            option.rect = QRect( p1.x() + abs(p1.x() - p2.x())/2 - 5, p1.y() - 5, 10, 10 );
            if ( chart->delegate() != 0 ) chart->delegate()->paint( &painter, option, chart->model()->index( 0, column ) );
        }
        painter.restore();
    }
}