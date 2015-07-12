#include "linearlegendview.h"
#include "private/linearlegendview_p.h"
#include "linearchartwidget.h"
#include "linearchart.h"
#include "../base/seriedelegates.h"
#include "../base/serieformat.h"

#include <QPainter>
#include <QWidget>
#include <QScrollBar>

namespace Thistle
{
LinearLegendView::LinearLegendView( LinearChart* chart, QWidget* parent ) : AbstractLegendView( new LinearLegendViewPrivate( this, chart ), parent )
{}


LinearLegendView::~LinearLegendView()
{}


void LinearLegendView::updateSizeHint( const QSize& source )
{
    Q_D( LinearLegendView );
    d->calculateSizeHint( source );
}


void LinearLegendView::paintSerie( QPainter& painter, int serie, const QRect& rect ) const
{
    const Q_D( LinearLegendView );

    QPoint p1( rect.left(), rect.center().y() );
    QPoint p2( rect.left() + 30, rect.center().y() );
    QPoint posText( rect.left() + 35, rect.top() );

    QString s( this->model()->headerData( serie, Qt::Horizontal ).toString() );
    SerieFormat style = d->associatedChart->serieFormat( serie );
    Thistle::Types t = style.type();

    bool isActive = this->isActiveColumn( serie );

    if ( !isActive )
    {
        QPen p = painter.pen();
        QColor c = p.color();
        c.setAlpha( c.alpha() * 0.5 );
        QPen pen( p );
        pen.setColor( c );
        painter.setPen( pen );
        painter.drawText( rect.translated( 40, -3 ), Qt::AlignVCenter, s );
        painter.setPen( p );
    }
    else
        painter.drawText( rect.translated( 40, -3 ), Qt::AlignVCenter, s );

    painter.save();

    QPen pen( style.pen() );
    pen.setWidth( 2 );

    if ( !isActive )
    {
        QColor c( pen.color() );
        c.setAlpha( c.alpha() * 0.5 );
        pen.setColor( c );
    }
    
    painter.setPen( pen );
    painter.setBrush( style.brush() );

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
        QBrush brush = painter.brush();
        painter.setBrush( Qt::NoBrush );
        QPainterPath path;
        QPoint p11 = p1 + QPoint( 0, 7 );
        QPoint p22 = p2 - QPoint( 0, 7 );
        path.moveTo( p1 );
        path.cubicTo( p11  + QPoint( 25, 0 ), p22 - QPoint( 25, 0 ), p22 );
        painter.drawPath( path );
        painter.setBrush( brush );
    }
    else if ( t.testFlag( Thistle::Bar ) )
    {
        int j = 0;
        Q_FOREACH( int i, QList<int>() << 15 << 8 << 17 << 5 )
        {
            painter.setPen( Qt::NoPen );
            QRect r( 10 + p1.x() + j * 8, rect.bottom() - i, 5, i );
            painter.drawRect( r );
            j += 1;
        }
    }

    if ( t.testFlag( Thistle::Dot ) )
    {
        QStyleOptionViewItem option;
        option.rect = QRect( p1.x() + abs(p1.x() - p2.x())/2 - 5, p1.y() - 5, 10, 10 );
        
        if ( isActive )
            d->dotDelegate->paintEnabled( &painter, option, this->model()->index( 0, serie ) );
        else
            d->dotDelegate->paintDisabled( &painter, option, this->model()->index( 0, serie ) );
    }
    painter.restore();
}


bool LinearLegendView::isActiveColumn( int column ) const
{
    bool isActive = true;
    if ( this->selectionModel() != 0 )
    {
        QModelIndexList selectedIndexes = this->selectionModel()->selectedIndexes();
        if ( !selectedIndexes.isEmpty() )
        {
            isActive = false;
            Q_FOREACH( QModelIndex idx, selectedIndexes )
            {
                if ( idx.column() == column )
                {
                    isActive = true;
                    break;
                }
            }
        }
    }
    return isActive;
}

}
