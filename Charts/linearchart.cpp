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
    along with Thistle.
    If not, see <http://www.gnu.org/licenses/>.
 Thistle    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/
#include "linearchart.h"
#include "delegates.h"
#include <QItemDelegate>
#include <QPainter>
#include <QPaintEvent>
#include <QItemDelegate>
#include <QDebug>
#include <QStyleOptionViewItem>

#include "Axis/orthogonalaxis.h"

namespace Thistle {

/*!
Constructs a linear chart with the given \c parent.
*/
LinearChart::LinearChart( QWidget* parent ) : AxisChart( new LinearChartPrivate( this ), parent ) {
    Q_D( LinearChart );
    this->setAxis( d->orthoAxis );
    d->pointDelegate->setParent( this );
    d->barDelegate->setParent( this );
    
}


QList<int> LinearChart::barStyleColumns() const {
    QList<int> bars;
    for ( int c = 0; c < this->model()->columnCount(); ++c ) {
        if ( columnType( c ) == Global::Bar ) {
            bars.append( c );
        }
    }
    return bars;
}


QList<int> LinearChart::calculateColumnsOrder() const {
    QList<int> areas;
    QList<int> bars;
    QList<int> lines;
    QList<int> points;
    for ( int i = 0; i < this->model()->columnCount(); ++i ) {
        Global::Types t = columnType( i );
        if ( t.testFlag( Global::Area ) ) {
            areas << i;
        } else if ( t.testFlag( Global::Bar ) ) {
            bars << i;
        } else if ( t.testFlag( Global::Dot ) ) {
            points << i;
        } else    {
            lines << i;
        }
    }
    areas << bars << lines << points;
    return areas;
}


Global::Types LinearChart::columnType( int column ) const {
    const Q_D( LinearChart );
    if ( d->style.contains( column ) ) {
        return d->style[ column ].type();
    }
    return SerieFormat().type();
}

QRectF LinearChart::itemRect( int row, int column, const QModelIndex& parent ) const {
    return this->itemRect( this->model()->index( row, column, parent ) );
}

QRectF LinearChart::itemRect( const QModelIndex& index ) const {
    /* Reimplemented from Chart.itemRect() */
    const Q_D( LinearChart );

    QRectF r;
    Global::Types t = this->columnType( index.column() );
    bool ok = false;
    qreal value = index.data().toReal( &ok );
    if ( ok == false ) {
        return QRectF();
    }
    QPointF pos = d->orthoAxis->valueToPoint( value, index.row() );
    QList<int> orderedColumns = this->calculateColumnsOrder();
    if ( t == Global::Bar ) {
        QList<int> bars = this->barStyleColumns();
        qreal margin = d->orthoAxis->stepSize() * 0.1;
        qreal w = float( d->orthoAxis->stepSize() - margin ) / bars.count();
        pos += QPointF( margin/2.0 + w * orderedColumns.indexOf( index.column() ), 0 );
        QPointF br( pos.x() + w, d->orthoAxis->origin().y() );
        r = QRectF( pos, br );
        if ( value < 0 ) {
            r.translate( 0, 1 );
        } else {
            r.translate( 0, -1 );
        }

        if ( d->orthoAxis->startOnAxis() == false ) {
            r.translate( -d->orthoAxis->stepSize()/2.0, 0 );
        }
    } else {
        r = QRectF( -7, -7, 14 ,14 ).translated( pos.x(), pos.y() );
    }
    return r.normalized().toRect();
}



void LinearChart::paintChart( QPainter& painter ) {
    const Q_D( LinearChart );

    painter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );
    d->orthoAxis->paintBack( painter );
    QList<int> ordered = this->calculateColumnsOrder();
    Q_FOREACH( int c, ordered ) {
        this->paintValues( painter, c );
    }
    d->orthoAxis->paintFront( painter );

    d->legend->paint( painter );
}


void LinearChart::paintSerieLegend(QPainter &painter, int column, QPoint pos, int maxHeight) const {
    const Q_D( LinearChart );
    QPoint p1 = pos + QPoint( 10, - maxHeight/2 );
    QPoint p2 = pos + QPoint( 40, - maxHeight/2 );
    QPoint posText = pos + QPoint( 45, 0 );
    QString s(model()->headerData( column, Qt::Horizontal ).toString() );
    SerieFormat style = serieFormat( column );
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
        d->pointDelegate->paint( &painter, option, this->model()->index( 0, column ) );
    }
    painter.restore();
}


void LinearChart::paintValues( QPainter& painter, int column ) {
    Q_D( LinearChart );
    Global::Types t = this->columnType( column );
 
    d->selectDelegate( t );

    int rows = this->model()->rowCount();
    painter.save();
    SerieFormat style = this->serieFormat( column );
    painter.setBrush( style.brush() );
    painter.setPen( style.pen() );

    bool isActive = true;
    if ( this->selectionModel() != 0 ) {
        QModelIndexList selectedIndexes = this->selectionModel()->selectedIndexes();
        if ( !selectedIndexes.isEmpty() ) {
            isActive = false;
            Q_FOREACH( QModelIndex idx, selectedIndexes ) {
                if ( idx.column() == column ) {
                    isActive = true;
                    break;
                }
            }
        }
    }

    d->paint( painter, column, t, isActive );

    painter.restore();
}


void LinearChart::updateRects() {
    Q_D( LinearChart );

    if ( this->model() == 0 ) {
                return;
    }

    this->scan();
    this->defineRects();
    d->orthoAxis->setValuesRect( QRect( d->orthoAxis->chartRect() ) );
    QFontMetrics m( this->font() );
    d->orthoAxis->valuesRect().setX( d->orthoAxis->valuesRect().x() + d->orthoAxis->yLabelsLength() );
    d->titleRect.moveTo( d->orthoAxis->chartRect().bottomLeft() );
    d->titleRect.translate( (d->orthoAxis->chartRect().width() - d->titleRect.width())/2, 20 );
    d->orthoAxis->update();
}

}