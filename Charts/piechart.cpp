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
#include "../kernel/Marb.h"
#include "abstractchart.h"

#include <QDebug>

#include <QPainter>
#include <QPaintEvent>

PieChart::PieChart( QWidget* parent ) :
    AbstractChart( parent ) {
    this->setEditTriggers( QAbstractItemView::NoEditTriggers );
    myRing = false;
    mySplitted = false;
    myStartAngle = 0.0;
}


qreal PieChart::startAngle() const {
    return myStartAngle;
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
    QPainterPath part;
    part.moveTo( myRect.center() );
    part.arcTo( myRect, -angle, -delta );
    if ( splitted == true ) {
        QPointF p = this->splittedOffset( angle, delta );
        part.translate( p.x(), p.y() );
    }
    part.closeSubpath();
    if ( myRing == true ) {
        QPainterPath p;
        p.addEllipse( myRect.center(), myRect.width() * 0.3, myRect.height() * 0.3 );
        part = part.subtracted( p );
    }
    return part;
}


QPainterPath PieChart::itemPath( const QModelIndex& index ) const {
    QPainterPath path;
    qreal angle = myStartAngle;
    for ( int r = 0; r < index.row(); ++r ) {
        QModelIndex id = this->model()->index( r, 0 );
        qreal v = qAbs( this->model()->data( id ).toReal() );
        qreal delta = 360.0 * v/myTotal;
        angle += delta;
    }
    qreal v = qAbs( this->model()->data( index ).toReal() );
    qreal delta = 360.0 * v/myTotal;
    if ( this->selectionModel()->selectedIndexes().contains( index )) {
        path = this->itemPart( angle, delta, true );
    } else {
        path = this->itemPart( angle, delta );
    }
    return path;
}


void PieChart::paintChart( QPainter &painter ) const {
    //this->updateChart();
    painter.save();
    int rows = this->model()->rowCount();
    qreal angle = myStartAngle;
    for ( int i = 0; i < rows; ++i ) {
        QModelIndex index = this->model()->index( i, 0 );
        QColor color( this->model()->data( index, Qt::DecorationRole ).toString() );
        if ( !color.isValid() ) {
            color = Marb::predefinedColor( i );
        }
        qreal v = qAbs( this->model()->data( index ).toReal() );
        qreal delta = 360.0 * v/myTotal;
        bool isSelected = this->selectionModel()->selectedIndexes().contains( index )
                                            || this->currentIndex() == index;
        if ( mySplitted == false ) {
            this->paintPart( painter, angle, delta, color, isSelected );
        } else {
            this->paintPartSplitted( painter, angle, delta, color, isSelected );
        }
        angle += delta;
    }
    painter.drawText( myTitleRect, Qt::AlignHCenter | Qt::TextWordWrap, myTitle );

    painter.restore();
}


/*Paint the legend in the QRect self._legendRect
The legend corresponds to the text in the Horizontal QHeaderView and the style defined for each column.
*/
void PieChart::paintLegend( QPainter& painter) const {
        painter.save();
        QFontMetrics metrics( this->font() );
        int metricsH = metrics.height();
        int h = metricsH + 10;
        int rows = this->model()->rowCount();
        int w = 0;
        int maxWidth = myLegendRect.width();
        QPoint legendPos( myLegendRect.topLeft() );
        QPoint pos = legendPos + QPoint( 50, 0);
        for (int r = 0; r < rows; ++r ) {
            QString s( this->model()->headerData( r, Qt::Vertical ).toString() );
            int sWidth = metrics.width( s ) + 50;
            QPoint p;
            if ( ( w + sWidth ) > maxWidth ) {
                int y = pos.y();
                p = QPoint( myLegendRect.x(), y + h );
                pos = QPoint( myLegendRect.x(), y + h );
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
        color = Marb::predefinedColor( serie );
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
    QPainterPath part = this->itemPart( angle, delta, true );
    painter.save();
    if ( mySplitted == true
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
    myRing = ring;
}


void PieChart::setSplitted( bool splitted ) {
    mySplitted = splitted;
}


void PieChart::setStartAngle( qreal angle ) {
    myStartAngle = angle;
    this->viewport()->update();
}


QPointF PieChart::splittedOffset( qreal angle, qreal delta ) const {
    QPainterPath part;
    part.moveTo( myRect.center() );
    part.arcTo( myRect, -angle, -delta );
    part.closeSubpath();
    QPointF p = part.pointAtPercent( 0.5 );
    QLineF line( p, myRect.center() );
    line.setLength( line.length() * 1.1 );
    p = line.p2();
    return myRect.center() - p;
}


void PieChart::updateRects() {
    if ( this->model() == 0 )    {
        return;
    }
    this->defineRects();
    //myTitleRect.moveTo( axis()->valuesRect.bottomLeft() );
    //myTitleRect.translate( (axis()->valuesRect.width() - myTitleRect.width())/2, 20 );
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
    int rows = this->model()->rowCount();

    myTotal = 0;
    for ( int r = 0; r < rows; ++r ) {
        QString s( this->model()->headerData( r, Qt::Vertical ).toString() );

        qreal value = this->model()->index( r, 0 ).data().toDouble();
        myTotal += value;
    }
    return 0;
}


void PieChart::defineRects() {
    myRect = this->contentsRect();
    this->calculateLegendRect( myRect );
    myRect.setWidth( myRect.width() - myLegendRect.width() - 10 );
    
    if ( myTitle != "" ) {
        QFont font = this->font();
        font.setItalic( true );
        QFontMetrics metrics( font );
        QRect r( 0, 0,myRect.width() - 40, 0 );
        myTitleRect = metrics.boundingRect( r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, myTitle );
        //myAxis->chartRect.setHeight( myAxis->chartRect.height() - myTitleRect.height() - 20 );
        myRect.setHeight( myRect.height() - myTitleRect.height() - 20 );
    }


    int w = qMin( myRect.width() * 0.80, myRect.height() * 0.80 );

    myRect.setWidth( w );
    myRect.setHeight( w );

    myTitleRect.moveTo( myTitleRect.x(), myRect.top() );
    
    myRect.moveTo( myRect.x(), myTitleRect.bottom() + 10 );

    myLegendRect.moveTo( myRect.right() + 10, myRect.top() );
    
}


void PieChart::calculateLegendRect( const QRect& source ) {
        QFontMetrics metrics( font() );
        int h = metrics.height() + 10;
        int rows = this->model()->rowCount();

        int w = 0;

        for( int r = 0; r < rows; ++r ) {
            QString s( this->model()->headerData( r, Qt::Vertical ).toString() );
            w = qMax( metrics.width( s ) + 40, w );
        }

        w = qMin( source.width(), w );
        
        myLegendRect = QRect( source.right() - w, this->contentsMargins().top(), w, rows * h );
}
