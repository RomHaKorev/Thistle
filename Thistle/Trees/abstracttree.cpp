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
#include "abstracttree.h"
#include "../kernel/itemdelegate.h"
#include <QScrollBar>
#include <QPainter>
#include <QPaintEvent>
#include <QStandardItem>
#include "../kernel/global.h"

namespace Thistle {

    AbstractTree::AbstractTree(QWidget *parent) : AbstractItemView( new AbstractTreePrivate(), parent ) {
        Q_D( AbstractTree );
        setItemDelegate( d->delegate );
    }

    AbstractTree::AbstractTree( AbstractTreePrivate* d, QWidget* parent ) : AbstractItemView( d, parent ) {
        setItemDelegate( d->delegate );
    }

    AbstractTree::~AbstractTree() {
    }


    QPen AbstractTree::connectionPen() const {
        const Q_D( AbstractTree );
        return d->connectionPen;
    }


    QModelIndex AbstractTree::indexAt(const QPoint &point) const {
        const Q_D( AbstractTree );
        QPoint p = point - QPoint( horizontalScrollBar()->value(), verticalScrollBar()->value() );
        Q_FOREACH( QModelIndex id, d->itemPos.keys() ) {
            QRectF r = itemRect( id );
            if ( r.contains( p ) ) {
                return id;
            }
        }
        return QModelIndex();
    }


    QPainterPath AbstractTree::itemPath( const QModelIndex& index ) const {
        QPainterPath path;
        path.addRect( this->itemRect( index ) );
        return path;
    }


    QRectF AbstractTree::itemRect( const QModelIndex& index ) const {
        const Q_D( AbstractTree );
        QPointF p = d->itemPos.value( index ) - QPointF( horizontalOffset(), verticalOffset() );
        return d->itemRect.translated( p.toPoint() );
    }


    void AbstractTree::paintConnections( QPainter& painter, const QPointF& offset ) const {
        const Q_D( AbstractTree );
        painter.save();
        painter.setPen( d->connectionPen );
        Q_FOREACH ( QModelIndex index, d->itemPos.keys() ) {
            paintConnectionsFor( painter, index, offset );
        }
        painter.restore();
    }


    void AbstractTree::paintConnectionsFor( QPainter& painter, const QModelIndex& index, const QPointF& offset ) const {
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


    void AbstractTree::paintEvent( QPaintEvent* event) {
        QPainter painter( viewport() );
        painter.setClipRect( event->rect() );
        painter.setRenderHint( QPainter::Antialiasing );
        paintConnections( painter, QPointF(0,0) );
        //paintItems( painter, myItemOffset );
        paintItems( painter, QPointF(0,0) );
    }


    void AbstractTree::paintItems( QPainter& painter, const QPointF& offset ) const {
        const Q_D( AbstractTree );
        Q_FOREACH ( QModelIndex index, d->itemPos.keys() ) {
            QStyleOptionViewItem option;
            option.rect = itemRect( index ).translated( offset.x(), offset.y() ).toRect();
            itemDelegate()->paint( &painter, option, index );
        }
    }


    void AbstractTree::resizeEvent( QResizeEvent* event ) {
        QAbstractItemView::resizeEvent( event );
        positionsInView();
    }


    void AbstractTree::setConnectionPen( const QPen& pen ) {
        Q_D( AbstractTree );
        d->connectionPen = pen;
    }


    void AbstractTree::setItemSize( const QSize& s ) {
        Q_D( AbstractTree );
        d->itemRect.setRect( -s.width() / 2, -s.height() / 2, s.width(), s.height() );
        this->positionsInView();
    }


    void AbstractTree::setItemSpacing(int w, int h) {
        Q_D( AbstractTree );
        d->xDistance = w;
        d->yDistance = h;
        this->positionsInView();
        this->update();
    }


    void AbstractTree::setX( const QModelIndex& index, qreal x ) {
        Q_D( AbstractTree );
        if ( !d->itemTreePos.contains( index ) ) {
            d->itemTreePos.insert( index, QPointF() );
        }
        d->itemTreePos[ index ].setX( x );
    }


    void AbstractTree::setY( const QModelIndex& index, qreal y ) {
        Q_D( AbstractTree );
        if ( !d->itemTreePos.contains( index ) ) {
            d->itemTreePos.insert( index, QPointF() );
        }
        d->itemTreePos[ index ].setY( y );
    }


    void AbstractTree::show() {
        this->positionsInView();
        QAbstractItemView::show();
    }


    void AbstractTree::updateValues() {
        this->positionsInTree();
    }

}