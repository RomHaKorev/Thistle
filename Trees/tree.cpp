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
  along with Marb.  If not, see <http://www.gnu.org/licenses/>.
 Marb  Copyright (C) 2013  Dimitry Ernot
*/
#include "tree.h"
#include "../marbitemdelegate.h"
#include <QScrollBar>
#include <QPainter>
#include <QPaintEvent>
#include <QStandardItem>
#include "../Marb.h"


Tree::Tree(QWidget *parent) : MarbAbstractItemView( parent ) {
  myRealSize = QSize( 200, 200);
  myItemRect = QRect( -40, -20, 80, 40 );
  myYDistance = 50;
  myXDistance = 20;
  myDepth = 0;
  myLeft = 0;
  myConnectionPen = QPen( QColor(Marb::LightGray), 2 );
  myDelegate = new MarbItemDelegate( this );
  setItemDelegate( myDelegate );
}


QPen Tree::connectionPen() const {
  return myConnectionPen;
}


QModelIndex Tree::indexAt(const QPoint &point) const {
  QPoint p = point - QPoint( horizontalScrollBar()->value(), verticalScrollBar()->value() );
  foreach( QModelIndex id, myItemPos.keys() ) {
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


QRectF Tree::itemRect(const QModelIndex &index) const {
  QPointF p = myItemPos.value( index ) - QPointF( horizontalOffset(), verticalOffset() );
  return myItemRect.translated( p.toPoint() );
}


void Tree::paintConnections( QPainter& painter, QPointF offset ) {
    painter.save();
    painter.setPen( myConnectionPen );
    foreach ( QModelIndex index, myItemPos.keys() ) {
        paintConnectionsFor( painter, index, offset );
    }
    painter.restore();
}


void Tree::paintConnectionsFor( QPainter& painter, QModelIndex index, QPointF offset ) {
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
**        PUBLIC SLOTS
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


void Tree::paintItems( QPainter& painter, QPointF offset ) {
    foreach ( QModelIndex index, myItemPos.keys() ) {
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
  myConnectionPen = pen;
}


void Tree::setItemSize( const QSize& s ) {
  myItemRect.setRect( -s.width() / 2, -s.height() / 2, s.width(), s.height() );
  positionsInView();
}


void Tree::setItemSpacing(int w, int h) {
  myXDistance = w;
  myYDistance = h;
  positionsInView();
  update();
}


void Tree::setX( QModelIndex index, qreal x ) {
  if ( !myItemTreePos.contains( index ) ) {
    myItemTreePos.insert( index, QPointF() );
  }
  myItemTreePos[ index ].setX( x );
}


void Tree::setY( QModelIndex index, qreal y ) {
  if ( !myItemTreePos.contains( index ) ) {
    myItemTreePos.insert( index, QPointF() );
  }
  myItemTreePos[ index ].setY( y );
}


void Tree::show() {
  positionsInView();
  QAbstractItemView::show();
}


void Tree::updateValues() {
  positionsInTree();
}


