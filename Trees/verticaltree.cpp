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
#include "verticaltree.h"
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QModelIndex>
#include <qmath.h>

namespace Thistle {

VerticalTree::ConnectorType VerticalTree::connectorType() const {
    const Q_D( VerticalTree );
    return (VerticalTree::ConnectorType)d->connectorType;
}


VerticalTree::VerticalTree( QWidget* parent ) : AbstractTree( parent ) {
    Q_D( VerticalTree );
    d->max = 0;
    d->min = 0;
    d->yDistance = 50;
    d->xDistance = 20;
    d->itemRect = QRect( -40, -20, 80, 40 );
    d->realSize = QSize( 100, 100 );
    d->connectorType = Straight;
    d->itemOffset = QPointF( 10, 10 );
}


void VerticalTree::paintConnectionsElbow( QPainter& painter, const QModelIndex& id ) const {
    const Q_D( VerticalTree );
    painter.save();
    painter.setPen( d->connectionPen );
    int rowCount = this->model()->rowCount( id );
    QRectF r = itemRect( id );
    if ( rowCount > 1 ) {
        QRectF child1 = itemRect( this->model()->index( 0, 0, id ) );
        QRectF child2 = itemRect( this->model()->index( rowCount - 1, 0, id ) );
        QPointF p1( child1.center().x(), child1.top() - d->yDistance / 2 );
        QPointF p2( child2.center().x(), child2.top() - d->yDistance / 2 );
        painter.drawLine( p1, p2 );
        p1 = QPointF( r.center().x(), r.bottom() + 1 );
        p2 = QPointF( r.center().x(), r.bottom() + d->yDistance / 2 );
        painter.drawLine( p1, p2 );
        for( int i = 0; i < this->model()->rowCount( id ); ++i ) {
            r = itemRect( this->model()->index( i, 0, id ) );
            p1 = QPointF( r.center().x(), r.top() );
            p2 = QPointF( r.center().x(), r.top() - d->yDistance / 2 );
            painter.drawLine( p1, p2 );
        }
    } else if ( rowCount == 1 ) {
        QPointF p1( r.center().x(), r.bottom() + 1 );
        QPointF p2( p1.x(), p1.y() + d->yDistance - 2 );
        painter.drawLine( p1, p2 );
    }
    painter.restore();
}


void VerticalTree::paintConnectionsFor( QPainter& painter, const QModelIndex& index, const QPointF& offset ) const {
    const Q_D( VerticalTree );
    painter.save();
    painter.setPen( d->connectionPen );
    if ( d->connectorType == Straight ) {
        AbstractTree::paintConnectionsFor( painter, index, offset );
    } else {
        int rows = this->model()->rowCount( index );
        QRectF r = itemRect(index).translated( offset.x(), offset.y() );
        if ( rows > 1 ) {
            QRectF child1 = itemRect( this->model()->index( 0, 0, index ) ).translated( offset.x(), offset.y() );
            QRectF child2 = itemRect( this->model()->index( rows - 1, 0, index ) ).translated( offset.x(), offset.y() );
            QPointF p1( child1.center().x(), child1.top() - d->yDistance / 2 );
            QPointF p2( child2.center().x(), child2.top() - d->yDistance / 2 );
            painter.drawLine( p1, p2 );
            p1 = QPointF( r.center().x(), r.bottom() + 1 );
            p2 = QPointF( r.center().x(), r.bottom() + d->yDistance / 2 );
            painter.drawLine( p1, p2 );
            for ( int i = 0; i < this->model()->rowCount(index); ++i ) {
                QRectF r = itemRect( this->model()->index( i, 0, index ) ).translated( offset.x(), offset.y() );
                p1 = QPointF( r.center().x(), r.top() );
                p2 = QPointF( r.center().x(), r.top() - d->yDistance / 2 );
                painter.drawLine( p1, p2 );
            }
        } else if ( rows == 1 ) {
                QPointF p1( r.center().x(), r.bottom() + 1 );
                QPointF p2( p1.x(), p1.y() + d->yDistance - 2 );
                painter.drawLine( p1, p2 );
        }
    }
    painter.restore();
}


void VerticalTree::paintConnectionsStraight( QPainter& painter, const QModelIndex& id ) const {
    const Q_D( VerticalTree );
    painter.save();
    painter.setPen( d->connectionPen );
    int rowCount = this->model()->rowCount( id );
    QPointF p1 = itemRect( id ).center();
    for( int i = 0; i < rowCount; ++i ) {
        QPointF p2 = itemRect( this->model()->index( i, 0, id ) ).center();
        painter.drawLine( p1, p2 );
    }
    painter.restore();
}
/***************************************
**
**                RESOLVING POSITIONS
**
***************************************/


void VerticalTree::positionsInTree() {
    Q_D( VerticalTree );
    d->itemTreePos.clear();
    d->depth = scan( this->model()->index(0,0), QPointF(0,0)).y();
    d->left = 0;
    //Q_FOREACH( QPointF p, myItemTreePos.values() ) {
    Q_FOREACH( QModelIndex idx, d->itemTreePos.keys() ) {
        QPointF p = d->itemTreePos[idx];
        d->left = qMax( d->left, p.x() );
    }
    d->depth -= 1;
    positionsInView();
}


void VerticalTree::positionsInView() {
    Q_D( VerticalTree );
    d->realSize.setWidth( ( d->left ) * ( d->xDistance + d->itemRect.width() ) + d->itemRect.width() );
    d->realSize.setHeight( d->depth * ( d->yDistance + d->itemRect.height() ) + d->itemRect.height() );
    setScrollBarValues();
    QPointF offset( d->itemRect.width()/2, d->itemRect.height()/2 );
    offset += d->itemOffset;
    Q_FOREACH( QModelIndex index, d->itemTreePos.keys() ) {
            QPointF p = d->itemTreePos[ index ];
            d->itemPos[index] = QPointF( p.x() * ( d->xDistance + d->itemRect.width() ), p.y() * ( d->yDistance + d->itemRect.height() ) ) + offset;
    }
}


QPointF VerticalTree::scan( const QModelIndex& index, const QPointF& ld ) {
    Q_D( VerticalTree );
    QPointF leftDepth( ld );
    int rows = this->model()->rowCount( index );
    if ( !index.isValid() ) {
        return QPointF(0, 0);
    } else if ( rows == 0 ) {
        setX( index, leftDepth.x() );
        setY(index, leftDepth.y());
        return QPointF(leftDepth.x() + 1, 1);
    }
    qreal childDepth = 0;
    for ( int r = 0; r < rows; ++r ) {
            QModelIndex child = index.child( r, 0 );
            QPointF p = scan( child, leftDepth + QPointF( 0, 1 ) );
            leftDepth.setX( p.x() );
            childDepth = qMax( childDepth, p.y() );
    }
    qreal left = d->itemTreePos[ this->model()->index(0, index.column(), index) ].x();
    qreal right = d->itemTreePos[ this->model()->index( rows - 1, index.column(), index ) ].x();
    if ( rows >= 2 ) {
            if ( rows % 2 == 1 ) {
                qreal r = qFloor( rows/2 ) + 1;
                qreal v = d->itemTreePos[ this->model()->index( r - 1, index.column(), index ) ].x();
                setX( index, v );
            } else {
                setX( index, (right + left) / 2 );
            }
    } else {
        setX( index, left );
    }
    setY(index, leftDepth.y());
    return QPointF( right + 1 , childDepth + 1 );
}


void VerticalTree::setConnectorType( VerticalTree::ConnectorType type ) {
    Q_D( VerticalTree );
    d->connectorType = type;
}


void VerticalTree::setScrollBarValues() {
    Q_D( VerticalTree );
    qreal dw = qMax( 0, ( (d->realSize.width() + d->itemRect.width() / 2 ) - width()) + 20);
    qreal dh = qMax( 0, (d->realSize.height() + d->itemRect.height()/2 - height()) + 20 );
    horizontalScrollBar()->setRange( 0, dw );
    verticalScrollBar()->setRange( 0, dh );
    d->itemOffset = QPointF( 10, 10 );
    qreal delta = width() - d->realSize.width();
    if ( delta > 20 ) {
        d->itemOffset = QPointF( delta/2.0, 10 );
    }
}


VerticalTree::~VerticalTree() {
}


}