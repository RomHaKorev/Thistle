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
#include "radialtree.h"
#include <QPaintEvent>
#include <QPainter>
#include <QAbstractItemModel>
#include <QFontMetrics>
#include <QScrollBar>
#include <qmath.h>
#include "../kernel/itemdelegate.h"

namespace Thistle {

RadialTree::RadialTree( QWidget* parent ) : AbstractTree( parent ) {
    Q_D( RadialTree );
    d->itemRect = QRect( -20, -20, 40, 40 );
    ItemStyle style = ItemStyle();
    style.setShape( Global::Ellipse );
    d->delegate = new ItemDelegate( this );
    d->delegate->setItemStyle( style );
    setItemDelegate( d->delegate );
    d->yDistance = 40;
    d->xDistance = 40;
    d->itemOffset = QPoint( 0, 0 );
    d->radius = 1;
    d->diagonal = 1;
}

RadialTree::RadialTree( RadialTreePrivate* d, QWidget* parent ) : AbstractTree( d, parent ) {
}


void RadialTree::paintCircles( QPainter& painter, const QPointF& offset ) const {
    const Q_D( RadialTree );
    painter.save();
    QColor c = d->connectionPen.color();
    c.setAlpha( c.alpha() * 0.15 );
    QPen pen( d->connectionPen );
    pen.setColor( c );
    pen.setWidth( pen.width() * 2 );
    painter.setPen( pen );
    painter.setRenderHint( QPainter::Antialiasing );
    QModelIndex index = this->model()->index(0,0);
    QPointF center = itemRect(index).translated( offset.x(), offset.y() ).center();
    for( int rd = 1; rd < d->depth; ++rd ) {
            qreal r = d->radius * rd / d->depth;
            painter.drawEllipse( center, r, r );
    }
    painter.restore();
}


void RadialTree::paintConnections( QPainter& painter, QPointF offset ) const  {
    paintCircles( painter, offset );
    AbstractTree::paintConnections( painter, offset );
}


void RadialTree::positionsInTree() {
    Q_D( RadialTree );
    d->itemTreePos.clear();
    d->orderedIndexes.clear();
    QPointF p = scan( this->model()->index(0,0) , QPointF(0,0) );
    d->left = p.x() + 1;
    d->depth = p.y();
    this->positionsInView();
}


void RadialTree::positionsInView() {
    Q_D( RadialTree );
    if ( d->itemTreePos.isEmpty() || (d->radius == HUGE_VAL) || (d->radius == 0) ) {
            return;
    }
    updatePerimeter();
    if ( (d->radius == HUGE_VAL) || (d->radius == 0) ) {
        return;
    }
    d->realSize.setWidth( d->radius * 2 + d->itemRect.width());
    d->realSize.setHeight( d->radius * 2 + d->itemRect.height() );
    setScrollBarValues();
    d->itemPos = d->itemTreePos;
    QPainterPath path;
    path.addEllipse( d->itemOffset + d->itemRect.center(), d->radius, d->radius );
    qreal l = 0;
    qreal factor = 0;
    Q_FOREACH( QModelIndex index, d->orderedIndexes ) {
        if ( this->model()->rowCount( index ) != 0 ) {
                    continue;
        }
        factor = qreal(d->depth) / qreal(d->itemPos[index].y());
        if ( this->model()->rowCount( index ) == 0 ) {
                l +=	(qreal(d->diagonal)/2.0) * qreal(factor);
        }
        qreal percent = path.percentAtLength( l );
        d->itemPos[index] = QPointF( percent, d->itemPos[index].y() );
        if ( this->model()->rowCount( index ) == 0 ) {
                l +=	(qreal(d->diagonal)/2.0) * qreal(factor);
        }
    }
    QList<QModelIndex> reversedOrderedIndexes;
    Q_FOREACH ( QModelIndex idx, d->orderedIndexes ) {
        reversedOrderedIndexes.prepend( idx );
    }
    Q_FOREACH ( QModelIndex index, reversedOrderedIndexes ) {
            if ( this->model()->rowCount( index ) == 0 ) {
                    continue;
            }
            QModelIndex left = this->model()->index( 0,0, index );
            QModelIndex right = this->model()->index( this->model()->rowCount(index) - 1,0, index );
            d->itemPos[index] = QPointF( float(d->itemPos[left].x() + d->itemPos[right].x()) / 2.0, d->itemPos[index].y() );
    }
    d->itemPos[model()->index(0,0)] = QPointF(0, 0);
    Q_FOREACH ( QModelIndex index, d->orderedIndexes ) {
        if ( d->depth == d->itemPos[index].y() ) {
                factor = 1.0;
        } else {
                if ( d->itemPos[index].y() != 0 ) {
                        factor = qreal(d->depth) / qreal(d->itemPos[index].y());
                } else {
                        factor = 1.0;
                }
        }
        qreal radius = qreal( d->radius ) / qreal(factor);
        QPainterPath circle;
        circle.addEllipse( d->itemRect.center(), radius, radius );
        d->itemPos[index] = circle.pointAtPercent( d->itemPos[index].x() ) + d->itemOffset;
    }
    d->itemPos[ this->model()->index(0,0) ] = d->itemOffset;
}


QPointF RadialTree::scan(QModelIndex index, QPointF leftDepth) {
    Q_D( RadialTree );
    d->orderedIndexes.append( index );
    int rows = this->model()->rowCount( index );
    if ( !index.isValid() ) {
        return QPointF();
    } else if ( rows == 0 ) {
            setX( index, leftDepth.x() );
            setY(index, leftDepth.y() );
            return QPointF(leftDepth.x() + 1, 1);
    }
    qreal childDepth = 0;
    for( int r = 0; r < rows; ++r ) {
            QModelIndex child = index.child( r, 0 );
            QPointF p = scan( child, leftDepth + QPointF( 0, 1 ) );
            leftDepth.setX( p.x() );
            childDepth = qMax( childDepth, p.y() );
    }
    qreal left = d->itemTreePos[ this->model()->index(0, 0, index) ].x();
    qreal right =    d->itemTreePos[ this->model()->index( rows - 1, 0, index ) ].x();
    if ( rows >= 2 ) {
            if ( rows % 2 == 1 ) {
                    qreal r = qFloor( rows/2 ) + 1;
                    qreal v = d->itemTreePos[ this->model()->index( r - 1, 0, index ) ].x();
                    setX( index, v );
            } else {
                    setX( index, ( right + left + 1 ) / 2.0 );
            }
    } else {
            setX( index, left );
    }
    setY(index, leftDepth.y());
    return QPointF( right + 1 , childDepth + 1 );
}


void RadialTree::setRotateText( bool rotate ) {
    Q_D( RadialTree );
    d->rotateText = rotate;
}
/***************************************
**
**                RESOLVING POSITIONS
**
***************************************/


void RadialTree::setScrollBarValues() {
    Q_D( RadialTree );
    QSize s = d->realSize + QSize(50, 50);
    qreal dw = qMax( 0, ( s.width() - width() )/2	);
    qreal dh = qMax ( 0, ( s.height() - height() )/2 );
//    qreal dw = qMax( 0, ( d->realSize.width() - width() )/2 + verticalScrollBar()->width() + 50 );
//    qreal dh = qMax ( 0, ( d->realSize.height() - height() )/2 + horizontalScrollBar()->height() + 50 );
    horizontalScrollBar()->setRange( -dw, dw );
    verticalScrollBar()->setRange( -dh, dh );
    d->itemOffset = QPoint( width() / 2, height() / 2 );
}


void RadialTree::updatePerimeter() {
    Q_D( RadialTree );
    qreal w = d->itemRect.width() + d->xDistance;
    qreal h = d->itemRect.height() + d->yDistance;
    d->diagonal = qSqrt( w * w + h * h );
    d->perimeter = 0;
    qreal factor = 1;
    Q_FOREACH ( QModelIndex index, d->itemTreePos.keys() ) {
        if ( this->model()->rowCount( index ) == 0 ) {
            factor = ( d->depth ) / ( d->itemTreePos.value( index ).y() );
            d->perimeter += d->diagonal * factor;
        }
    }
    d->radius = d->perimeter / ( 2 * M_PI );
    if ( d->radius*2 < d->diagonal * d->depth ) {
        d->radius = ( d->diagonal * d->depth )/2 * 1.5;
    }
}


}