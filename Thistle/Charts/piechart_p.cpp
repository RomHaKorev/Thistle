#include "piechart_p.h"

#include "abstractchart.h"
#include "piechart.h"
#include "legends/piechartlegend.h"
#include <QFontMetrics>
#include <QDebug>

namespace Thistle {

PieChartPrivate::PieChartPrivate( bool is3D, PieChart* q ) : AbstractChartPrivate( q )  {
    offsetFactor = 1.2;
    ring = false;
    splitted = false;
    startAngle = 0.0;
    delete this->legend;
    this->legend = new PieChartLegend( q );
    this->is3D = false;
}

void PieChartPrivate::createRects( const QRect& area ) {

    this->calculateLegendRect( area );

    this->rect = area;
    this->rect.setWidth( this->rect.width() - ( this->legend->area.width() + 15 ) );

    if ( this->title != "" ) {
        QFont font = q_ptr->font();
        font.setItalic( true );
        QFontMetrics metrics( font );
        QRect r( 0, 0, this->rect.width() - 40, 0 );
        this->titleRect = metrics.boundingRect( r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, this->title );
        this->rect.setHeight( this->rect.height() - this->titleRect.height() - 20 );
    }


    int w = qMin( this->rect.width(), this->rect.height() );

    this->rect.setWidth( w );
    this->rect.setHeight( w );

    /*QPoint offset = area.center() - this->rect.center();
    this->rect.translate( offset );*/

    this->titleRect.moveTo( this->rect.x(), this->rect.top() );
    this->titleRect.setWidth( this->rect.width() );
    this->rect.moveTo( this->rect.x(), this->titleRect.bottom() + 10 );

    this->legend->area.moveLeft( this->rect.right() + 15 );

    this->rect.translate( this->rect.width() * 0.10,  this->rect.height() * 0.10 );
    this->rect.setWidth( this->rect.width() * 0.80 );
    this->rect.setHeight( this->rect.height() * 0.80 );
    
    if ( this->is3D ) {
        createRects3D( area );
    }
}

void PieChartPrivate::createRects3D( const QRect& area ) {
    Q_Q( PieChart );
    //PieChart* q = reinterpret_cast<PieChart*>( q_ptr );
    if ( q->model() == 0 ) {
        return;
    }

    if ( this->splitted == true ) {
        this->rect.translate( 10, 10 );
        this->rect.setWidth( this->rect.width() - 20 );
        this->rect.setHeight( this->rect.height() - 20 );
    }
    this->height = this->rect.height() * 0.20;
    this->rect.setHeight( this->rect.height() * 0.75 );
    this->angles.clear();
    qreal angle = this->startAngle;
    for ( int i = 0; i < q->model()->rowCount(); ++i ) {
        this->angles << angle;
        QModelIndex index = q->model()->index( i, 0 );
        qreal v = qAbs( q->model()->data( index ).toReal() );
        qreal delta = 360.0 * v/this->total;
        this->angles << delta;
        angle += delta;
    }
    this->angles << 360.0;
}

void PieChartPrivate::calculateLegendRect( const QRect& source ) {
    Q_Q( PieChart );
    QFontMetrics metrics( q->font() );
    int h = metrics.height() + 10;
    int rows = q->model()->rowCount();

    int w = 0;

    for( int r = 0; r < rows; ++r ) {
        QString s( q->model()->headerData( r, Qt::Vertical ).toString() );
        w = qMax( metrics.width( s ) + 40, w );
    }

    w = qMin( source.width(), w );
        
    this->legend->area = QRect( source.right() - w, q->contentsMargins().top(), w, rows * h );
}


}
