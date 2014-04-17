/*
 This file is part of Marb.
    Marb is free software: you can redistribute it and/or modify
    it under the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.
    Marb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    Lesser GNU General Public License for more details.
    You should have received a copy of the Lesser GNU General Public License
    along with Marb.    If not, see <http://www.gnu.org/licenses/>.
 Marb    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/


#include "piechart.h"
#include "../kernel/global.h"
#include "abstractchart.h"

#include <QDebug>

#include <QPainter>
#include <QPaintEvent>

namespace Marb {

PieChart::PieChart( QWidget* parent ) :
    AbstractChart( new PieChartPrivate(), parent ) {
    this->setEditTriggers( QAbstractItemView::NoEditTriggers );
}


qreal PieChart::startAngle() const {
    const Q_D( PieChart );
    return d->startAngle;
}


void PieChart::configureColor(QPainter &painter, QColor base, int flag ) const {
    switch ( flag ) {
        case 0:
        default:
            painter.setPen( QPen( base.darker( 105 ), 4 ) );
            painter.setBrush( base );
        break;
        case 1:
            painter.setPen( QPen( base.darker( 105 ), 2 ) );
            painter.setBrush( base );
        break;
        case 2:
            base.setAlpha( base.alpha() * 0.75 );
            painter.setPen( QPen( base.lighter( 110 ), 2 ) );
            painter.setBrush( base.lighter( 120 ) );
    }
}


QPainterPath PieChart::itemPart( qreal angle, qreal delta, bool splitted ) const {
    const Q_D( PieChart );
    QPainterPath part;
    part.moveTo( d->rect.center() );
    part.arcTo( d->rect, -angle, -delta );
    if ( splitted == true ) {
        QPointF p = this->splittedOffset( angle, delta );
        part.translate( p.x(), p.y() );
    }
    part.closeSubpath();
    if ( d->ring == true ) {
        QPainterPath p;
        p.addEllipse( d->rect.center(), d->rect.width() * 0.3, d->rect.height() * 0.3 );
        part = part.subtracted( p );
    }
    return part;
}


QPainterPath PieChart::itemPath( const QModelIndex& index ) const {
    const Q_D( PieChart );
    QPainterPath path;
    qreal angle = d->startAngle;
    for ( int r = 0; r < index.row(); ++r ) {
        QModelIndex id = this->model()->index( r, 0 );
        qreal v = qAbs( this->model()->data( id ).toReal() );
        qreal delta = 360.0 * v/d->total;
        angle += delta;
    }
    qreal v = qAbs( this->model()->data( index ).toReal() );
    qreal delta = 360.0 * v/d->total;
    if ( this->selectionModel()->selectedIndexes().contains( index )) {
        path = this->itemPart( angle, delta, true );
    } else {
        path = this->itemPart( angle, delta );
    }
    return path;
}


void PieChart::paintChart( QPainter &painter ) const {
    const Q_D( PieChart );
    //this->updateChart();
    painter.save();
    int rows = this->model()->rowCount();
    qreal angle = d->startAngle;
    for ( int i = 0; i < rows; ++i ) {
        QModelIndex index = this->model()->index( i, 0 );
        QColor color( this->model()->data( index, Qt::DecorationRole ).toString() );
        if ( !color.isValid() ) {
            color = Color::predefinedColor( i );
        }
        qreal v = qAbs( this->model()->data( index ).toReal() );
        qreal delta = 360.0 * v/d->total;
        bool isSelected = this->selectionModel()->selectedIndexes().contains( index )
                                            || this->currentIndex() == index;
        if ( d->splitted == false ) {
            this->paintPart( painter, angle, delta, color, isSelected );
        } else {
            this->paintPartSplitted( painter, angle, delta, color, isSelected );
        }
        angle += delta;
    }
    painter.drawText( d->titleRect, Qt::AlignHCenter | Qt::TextWordWrap, d->title );

    painter.restore();
}


/*Paint the legend in the QRect self._legendRect
The legend corresponds to the text in the Horizontal QHeaderView and the style defined for each column.
*/
void PieChart::paintLegend( QPainter& painter) const {
    const Q_D( PieChart );
    painter.save();
    QFontMetrics metrics( this->font() );
    int metricsH = metrics.height();
    int h = metricsH + 10;
    int rows = this->model()->rowCount();
    int w = 0;
    int maxWidth = d->legendRect.width();
    QPoint legendPos( d->legendRect.topLeft() );
    QPoint pos = legendPos + QPoint( 50, 0);
    for (int r = 0; r < rows; ++r ) {
        QString s( this->model()->headerData( r, Qt::Vertical ).toString() );
        int sWidth = metrics.width( s ) + 50;
        QPoint p;
        if ( ( w + sWidth ) > maxWidth ) {
            int y = pos.y();
            p = QPoint( d->legendRect.x(), y + h );
            pos = QPoint( d->legendRect.x(), y + h );
            w = sWidth;
            pos += QPoint( sWidth, 0 );
        } else {
            p = pos + QPoint( -40,    0 );
            w += sWidth;
            pos += QPoint( sWidth, 0 );
        }
        this->paintSerieLegend(painter, r, p, metricsH);
    }
    painter.restore();
}


void PieChart::paintSerieLegend( QPainter& painter, int serie, QPoint pos, int metricsH ) const {

    QColor color( this->model()->data( this->model()->index( serie, 0 ), Qt::DecorationRole ).toString() );
    if ( !color.isValid() ) {
        color = Color::predefinedColor( serie );
    }

    QRect r( pos.x(), pos.y() - 20, 14, 14 );
    QPoint posText = pos + QPoint( 20, -metricsH/2 );

    QString s = this->model()->headerData( serie, Qt::Vertical ).toString();
    painter.drawText( posText, s );
    painter.save();
    painter.setBrush( color );
    painter.setPen( QPen( color.darker( 100 ), 1 ) );
    painter.drawRect( r );

    painter.restore();
}

void PieChart::paintPart( QPainter& painter, qreal angle, qreal delta, QColor color, bool isSelected ) const {
    if ( isSelected == true ) {
        this->paintPartSplitted( painter, angle, delta, color );
        return;
    }
    QPainterPath part = this->itemPart( angle, delta );
    painter.save();
    painter.setClipPath( part ); /* To avoid the "borders superposition" */
    int flag = 0;
    this->configureColor( painter, color, flag );
    painter.drawPath( part );
    painter.restore();
}


void PieChart::paintPartSplitted( QPainter &painter, qreal angle, qreal delta,
                                  QColor color, bool isSelected ) const {
    const Q_D( PieChart );
    QPainterPath part = this->itemPart( angle, delta, true );
    painter.save();
    if ( d->splitted == true
             && ( !this->selectionModel()->selectedIndexes().isEmpty() || this->currentIndex().isValid() )
             && isSelected == false ) {
        this->configureColor( painter, color, 2 );
    } else {
        this->configureColor( painter, color, 1 );
    }
    painter.drawPath( part );
    painter.restore();
}


void PieChart::scrollTo(const QModelIndex &/*index*/, ScrollHint /*hint*/) {
}


void PieChart::setRing( bool ring ) {
    Q_D( PieChart );
    d->ring = ring;
}


void PieChart::setSplitted( bool splitted ) {
    Q_D( PieChart );
    d->splitted = splitted;
}


void PieChart::setStartAngle( qreal angle ) {
    Q_D( PieChart );
    d->startAngle = angle;
    this->viewport()->update();
}


QPointF PieChart::splittedOffset( qreal angle, qreal delta ) const {
    const Q_D( PieChart );
    QPainterPath part;
    part.moveTo( d->rect.center() );
    part.arcTo( d->rect, -angle, -delta );
    part.closeSubpath();
    QPointF p = part.pointAtPercent( 0.5 );
    QLineF line( p, d->rect.center() );
    line.setLength( line.length() * 1.1 );
    p = line.p2();
    return d->rect.center() - p;
}


void PieChart::updateRects() {
    if ( this->model() == 0 )    {
        return;
    }
    this->defineRects();
    //myTitleRect.moveTo( axis()->myValuesRect.bottomLeft() );
    //myTitleRect.translate( (axis()->myValuesRect.width() - myTitleRect.width())/2, 20 );
}


QRect PieChart::visualRect( const QModelIndex& index ) const {
    Q_UNUSED(index)
    /* To force to repaint the whole chart. Not only the area of the part*/
    return QRect( 0, 0, width(), height() );
}


int PieChart::scan() {
    /*Scans values in the model to find the minimum and the maximum. Returns the width needed to display the Y scale.
    If the values are greater than zero, the minimum is equal to 0. If the values are less than 0, the maximum is equal to 0.
    If a value is not a number (undefined, a string, etc.), she's considered as equal to 0. */

    Q_D( PieChart );

    int rows = this->model()->rowCount();

    d->total = 0;
    for ( int r = 0; r < rows; ++r ) {
        QString s( this->model()->headerData( r, Qt::Vertical ).toString() );

        qreal value = this->model()->index( r, 0 ).data().toDouble();
        d->total += value;
    }
    return 0;
}


void PieChart::defineRects() {
    Q_D( PieChart );
    d->rect = this->contentsRect();
    this->calculateLegendRect( d->rect );
    d->rect.setWidth( d->rect.width() - d->legendRect.width() - 10 );
    
    if ( d->title != "" ) {
        QFont font = this->font();
        font.setItalic( true );
        QFontMetrics metrics( font );
        QRect r( 0, 0, d->rect.width() - 40, 0 );
        d->titleRect = metrics.boundingRect( r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, d->title );
        //myAxis->chartRect.setHeight( myAxis->chartRect.height() - myTitleRect.height() - 20 );
        d->rect.setHeight( d->rect.height() - d->titleRect.height() - 20 );
    }


    int w = qMin( d->rect.width() * 0.80, d->rect.height() * 0.80 );

    d->rect.setWidth( w );
    d->rect.setHeight( w );

    d->titleRect.moveTo( d->titleRect.x(), d->rect.top() );
    d->rect.moveTo( d->rect.x(), d->titleRect.bottom() + 10 );
    d->legendRect.moveTo( d->rect.right() + 10, d->rect.top() );
    
}


void PieChart::calculateLegendRect( const QRect& source ) {
    Q_D( PieChart );
    QFontMetrics metrics( font() );
    int h = metrics.height() + 10;
    int rows = this->model()->rowCount();

    int w = 0;

    for( int r = 0; r < rows; ++r ) {
        QString s( this->model()->headerData( r, Qt::Vertical ).toString() );
        w = qMax( metrics.width( s ) + 40, w );
    }

    w = qMin( source.width(), w );
        
    d->legendRect = QRect( source.right() - w, this->contentsMargins().top(), w, rows * h );
}

}