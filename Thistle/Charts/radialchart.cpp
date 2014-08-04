/*
This file is part of Thistle.
Thistle is free software: you can redistribute it and/or modify
it under the terms of the Lesser GNU General Public License as published by
the Free Software Foundation, either version 3 of the License.
Thistle is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
Lesser GNU General Public License for more details.
You should have received a copy of the Lesser GNU General Public License
along with Thistle.    If not, see <http://www.gnu.org/licenses/>.
Thistle    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/

#include "radialchart.h"
#include "Axis/radialaxis.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

namespace Thistle {

    RadialChart::RadialChart( QWidget* parent ) : AxisChart( new RadialChartPrivate( this ), parent ) {
        Q_D( RadialChart );
        d->radialAxis = new RadialAxis();

        this->setAxis( d->radialAxis );
    }

    RadialChart::RadialChart( RadialChartPrivate* d, QWidget* parent ) : AxisChart( d, parent ) {
        d->radialAxis = new RadialAxis();

        this->setAxis( d->radialAxis );
    }


    QRectF RadialChart::itemRect(const QModelIndex &index) const {
        Q_UNUSED( index )
            return QRect();
    }


    QPainterPath RadialChart::itemPath( const QModelIndex& index ) const {
        /*Returns the bounding path for the item pointed by the [index].
        *index: QModelIndex*/
        const Q_D( RadialChart );
        QPainterPath path;
        float angle = float(d->radialAxis->stepSize() / this->model()->columnCount() );
        float delta = 0.1 * angle;
        float startAngle = angle * index.column();
        startAngle += index.row() * d->radialAxis->stepSize() + d->radialAxis->startAngle();
        QPainterPath pathCenter;
        QRect rectangle( -d->radialAxis->centerHoleDiam()/2, -d->radialAxis->centerHoleDiam()/2, d->radialAxis->centerHoleDiam(), d->radialAxis->centerHoleDiam() );
        rectangle.translate( d->radialAxis->valuesRect().center() );
        pathCenter.addEllipse( rectangle );
        qreal value = index.data().toReal();

        rectangle = d->radialAxis->valueToRect( value );

        path.moveTo( d->radialAxis->valuesRect().center() );
        path.arcTo( rectangle, (startAngle + delta), (angle - delta * 2) );
        path.closeSubpath();
        path = path.subtracted( pathCenter );
        return path;
    }


    void RadialChart::paintAxis( QPainter& painter ) const {
        const Q_D( RadialChart );
        painter.save();
        //paintTicks( painter );
        painter.setRenderHint( QPainter::Antialiasing, false );
        painter.setPen( QPen( Qt::darkGray, 1.5 ) );
        QPoint p1 = d->radialAxis->valuesRect().center();
        QPoint p2 = p1 + QPoint( d->radialAxis->valuesRect().width()/2, 0 );
        painter.drawLine( p1, p2 );
        painter.restore();
    }


    void RadialChart::paintChart( QPainter& painter ) {
        const Q_D( RadialChart );
        if ( this->model() == 0 ) {
            return;
        }
        painter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );
        d->radialAxis->paintBack( painter );
        int cols = this->model()->columnCount();
        painter.save();
        for ( int c = 0; c < cols; ++c ) {
            SerieFormat style = serieFormat( c );
            painter.setPen( style.pen() );
            painter.setBrush( style.brush() );
            this->paintSerie( painter, c );
        }
        painter.restore();
        d->radialAxis->paintFront( painter );
        //this->paintLegend( painter );
        d->legend->paint( painter );
        painter.drawText( d->titleRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, d->title );
    }


    void RadialChart::paintSerie( QPainter& painter, int column ) const {
        int rows = this->model()->rowCount();
        painter.save();
        bool isActive = false;
        QModelIndexList selectedIndexes = this->selectionModel()->selectedIndexes();
        if ( selectedIndexes.isEmpty() != false ) {
            Q_FOREACH( QModelIndex idx, selectedIndexes ) {
                if ( idx.column() == column ) {
                    isActive = true;
                    break;
                }
            }
        } else {
            isActive = true;
        }
        for ( int r = 0; r < rows; ++r ) {
            QModelIndex index = this->model()->index( r, column );
            painter.save();
            if ( !selectedIndexes.isEmpty() ) {
                if ( !selectedIndexes.contains( index ) ) {
                    QPen pen = painter.pen();
                    if ( isActive == false ) {
                        QColor c( Thistle::Colors::Gray );
                        c.setAlpha( 125 );
                        pen.setColor( c );
                        painter.setPen( pen );
                        c = QColor( Thistle::Colors::LightGray );
                        c.setAlpha( 150 );
                        painter.setBrush( QBrush( c ) );
                    } else {
                        QColor c = pen.color();
                        c.setAlpha( c.alpha() * 0.5 );
                        pen.setColor( c );
                        painter.setPen( pen );
                        QBrush brush = painter.brush();
                        c = brush.color();
                        c.setAlpha( c.alpha() * 0.5 );
                        brush.setColor( c );
                        painter.setBrush( brush );
                    }
                }
            }
            QPainterPath path = this->itemPath( index );
            painter.drawPath( path );
            painter.restore();
        }
        painter.restore();
    }


/*    void RadialChart::setAlphaBeta() {
        Q_D( RadialChart );
        qreal w = d->radialAxis->valuesRect().width();
        qreal m = w * 0.3;
    }*/


    void RadialChart::updateRects() {
        Q_D( RadialChart );
        if ( this->model() == 0 ) {
            return;
        }
        d->radialAxis->setModel( this->model() );
        this->scan();
        this->defineRects();
        qreal w = qMin( d->radialAxis->chartRect().width(), d->radialAxis->chartRect().height() );
        d->radialAxis->setValuesRect( QRect( -w/2, -w/2, w, w ) );
        d->radialAxis->valuesRect().translate( d->radialAxis->chartRect().center().x(), d->radialAxis->chartRect().center().y() );
        d->titleRect.moveTo( d->radialAxis->chartRect().bottomLeft() );
        d->titleRect.translate( (d->radialAxis->chartRect().width() - d->titleRect.width())/2, 10 );

        d->radialAxis->update();
    }

}