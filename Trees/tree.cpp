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
#include "tree.h"
#include "../kernel/itemdelegate.h"
#include <QScrollBar>
#include <QPainter>
#include <QPaintEvent>
#include <QStandardItem>
#include "../kernel/global.h"


Tree::Tree(QWidget *parent) : AbstractItemView( new TreePrivate(), parent ) {
    Q_D( Tree );
    setItemDelegate( d->delegate );
}

Tree::Tree( TreePrivate* d, QWidget* parent ) : AbstractItemView( d, parent ) {
    setItemDelegate( d->delegate );
}


QPen Tree::connectionPen() const {
    const Q_D( Tree );
    return d->connectionPen;
}


QModelIndex Tree::indexAt(const QPoint &point) const {
    const Q_D( Tree );
    QPoint p = point - QPoint( horizontalScrollBar()->value(), verticalScrollBar()->value() );
    Q_FOREACH( QModelIndex id, d->itemPos.keys() ) {
        QRectF r = itemRect( id );
        if ( r.contains( p ) ) {
            return id;
        }
    }
    return QModelIndex();
}


QPainterPath Tree::itemPath( const QModelIndex& index ) const {
    QPainterPath path;
    path.addRect( this->itemRect( index ) );
    return path;
}


QRectF Tree::itemRect( const QModelIndex& index ) const {
    const Q_D( Tree );
    QPointF p = d->itemPos.value( index ) - QPointF( horizontalOffset(), verticalOffset() );
    return d->itemRect.translated( p.toPoint() );
}


void Tree::paintConnections( QPainter& painter, const QPointF& offset ) const {
    const Q_D( Tree );
    painter.save();
    painter.setPen( d->connectionPen );
    Q_FOREACH ( QModelIndex index, d->itemPos.keys() ) {
            paintConnectionsFor( painter, index, offset );
    }
    painter.restore();
}


void Tree::paintConnectionsFor( QPainter& painter, const QModelIndex& index, const QPointF& offset ) const {
        painter.save();
        QModelIndex parent = this->model()->parent( index );
        if ( parent.isValid() ) {
                QPointF p1 = itemRect( index ).translated( offset.x(), offset.y() ).center();
                QPointF p2 = itemRect( parent ).translated( offset.x(), offset.y() ).center();
                painter.drawLine( p1, p2 );
        }
        painter.restore();
}
/***************************************
**
**                PUBLIC SLOTS
**
***************************************/


void Tree::paintEvent( QPaintEvent* event) {
    QPainter painter( viewport() );
    painter.setClipRect( event->rect() );
    painter.setRenderHint( QPainter::Antialiasing );
    paintConnections( painter, QPointF(0,0) );
    //paintItems( painter, myItemOffset );
    paintItems( painter, QPointF(0,0) );
}


void Tree::paintItems( QPainter& painter, const QPointF& offset ) const {
    const Q_D( Tree );
    Q_FOREACH ( QModelIndex index, d->itemPos.keys() ) {
            QStyleOptionViewItem option;
            option.rect = itemRect( index ).translated( offset.x(), offset.y() ).toRect();
            itemDelegate()->paint( &painter, option, index );
    }
}


void Tree::resizeEvent( QResizeEvent* event ) {
    QAbstractItemView::resizeEvent( event );
    positionsInView();
}


void Tree::setConnectionPen( const QPen& pen ) {
    Q_D( Tree );
    d->connectionPen = pen;
}


void Tree::setItemSize( const QSize& s ) {
    Q_D( Tree );
    d->itemRect.setRect( -s.width() / 2, -s.height() / 2, s.width(), s.height() );
    this->positionsInView();
}


void Tree::setItemSpacing(int w, int h) {
    Q_D( Tree );
    d->xDistance = w;
    d->yDistance = h;
    this->positionsInView();
    this->update();
}


void Tree::setX( QModelIndex index, qreal x ) {
    Q_D( Tree );
    if ( !d->itemTreePos.contains( index ) ) {
        d->itemTreePos.insert( index, QPointF() );
    }
    d->itemTreePos[ index ].setX( x );
}


void Tree::setY( QModelIndex index, qreal y ) {
    Q_D( Tree );
    if ( !d->itemTreePos.contains( index ) ) {
        d->itemTreePos.insert( index, QPointF() );
    }
    d->itemTreePos[ index ].setY( y );
}


void Tree::show() {
    this->positionsInView();
    QAbstractItemView::show();
}


void Tree::updateValues() {
    this->positionsInTree();
}


