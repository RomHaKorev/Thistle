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
#include "../Marb.h"
#include "Axis/orthogonalaxis.h"

#include <QDebug>

#include <QPainter>
#include <QPaintEvent>

PieChart::PieChart( QWidget* parent ) :
    AxisChart( parent ) {
    this->setEditTriggers( QAbstractItemView::NoEditTriggers );
    myRing = false;
    mySplitted = false;
    myStartAngle = 0.0;
    myLegend = QString();
    myPieAxis = new OrthogonalAxis();
    setAxis( myPieAxis );
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


void PieChart::paintChart( QPainter &painter ) {
    this->updateChart();
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
    painter.drawText( 10, myRect.bottomLeft().y() + 10,
                                        width() - 20, height() - myRect.height(), Qt::AlignHCenter | Qt::TextWordWrap, myLegend );
    painter.restore();
}


void PieChart::paintEvent(QPaintEvent *event) {
    Q_UNUSED( event )
    if ( this->model() == 0 ) {
        return;
    }
    QPainter painter( viewport() );
    painter.setRenderHint( QPainter::Antialiasing );
    paintChart( painter );
}


void PieChart::paintPart( QPainter& painter, qreal angle, qreal delta, QColor color, bool isSelected ) {
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
                                                                    QColor color, bool isSelected ) {
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


bool PieChart::save( QString filename ) {
    QPixmap pix( size() );
    pix.fill( Qt::white );
    QPainter painter( &pix );
    painter.setRenderHint( QPainter::Antialiasing );
    this->paintChart( painter );
    return pix.save( filename );
}


void PieChart::scrollTo(const QModelIndex &/*index*/, ScrollHint /*hint*/) {
}


void PieChart::setLegend( QString legend ) {
    myLegend = legend;
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


void PieChart::updateChart() {
    qreal w = ( width() - 40 );
    qreal h = ( height() - 40 );
    if ( !myLegend.isNull() ) {
        QFontMetrics metrics( font() );
        QRect r = metrics.boundingRect( 10, 0, width() - 20, height(), Qt::AlignHCenter | Qt::TextWordWrap, myLegend );
        h -=    10 + r.height();
    }
    if ( w < h ) {
        myRect = QRect( 20, 20 + (h-w)/2, w , w );
    } else {
        myRect = QRect( 20 + (w-h)/2, 20, h , h );
    }
    myRect.translate( myRect.width() * 0.05, myRect.height() * 0.05 );
    myRect.setWidth( 0.9 * myRect.width() );
    myRect.setHeight( 0.9 * myRect.height() );
    myTotal = 0;
    for ( int i = 0; i < this->model()->rowCount(); ++i ) {
        QModelIndex index = this->model()->index( i, 0 );
        myTotal += qAbs( this->model()->data( index ).toReal() );
    }
}


void PieChart::updateRects() {
    if ( this->model() == 0 )    {
        return;
    }
    this->defineRects();
    myTitleRect.moveTo( axis()->valuesRect.bottomLeft() );
    myTitleRect.translate( (axis()->valuesRect.width() - myTitleRect.width())/2, 20 );
}


QRect PieChart::visualRect( const QModelIndex& index ) const {
    Q_UNUSED(index)
    /* To force to repaint the whole chart. Not only the area of the part*/
    return QRect( 0, 0, width(), height() );
}


