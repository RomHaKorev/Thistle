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

#include "verticaltree.h"

#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QModelIndex>
#include <qmath.h>

VerticalTree::VerticalTree( QWidget* parent ) : Tree( parent ) {
  myMax = 0;
  myMin = 0;
  myYDistance = 50;
  myXDistance = 20;
  myItemRect = QRect( -40, -20, 80, 40 );
  myRealSize = QSize( 100, 100 );
  myConnectorType = Straight;
  myItemOffset = QPointF( 10, 10 );
}

VerticalTree::~VerticalTree() {

}

void VerticalTree::setScrollBarValues() {
  qreal dw = qMax( 0, ( (myRealSize.width() + myItemRect.width() / 2 ) - width()) + 20);
  qreal dh = qMax( 0, (myRealSize.height() + myItemRect.height()/2 - height()) + 20 );
  horizontalScrollBar()->setRange( 0, dw );
  verticalScrollBar()->setRange( 0, dh );
  myItemOffset = QPointF( 10, 10 );
  qreal delta = width() - myRealSize.width();

  if ( delta > 20 ) {
    myItemOffset = QPointF( delta/2.0, 10 );
  }


}

void VerticalTree::paintConnectionsFor( QPainter& painter, QModelIndex index, QPointF offset ) {
        painter.save();
        painter.setPen( myConnectionPen );

        if ( myConnectorType == Straight ) {
            Tree::paintConnectionsFor( painter, index, offset );
        } else {
            int rows = this->model()->rowCount( index );
            QRectF r = itemRect(index).translated( offset.x(), offset.y() );
            if ( rows > 1 ) {
                QRectF child1 = itemRect( this->model()->index( 0, 0, index ) ).translated( offset.x(), offset.y() );
                QRectF child2 = itemRect( this->model()->index( rows - 1, 0, index ) ).translated( offset.x(), offset.y() );
                QPointF p1( child1.center().x(), child1.top() - myYDistance / 2 );
                QPointF p2( child2.center().x(), child2.top() - myYDistance / 2 );
                painter.drawLine( p1, p2 );

                p1 = QPointF( r.center().x(), r.bottom() + 1 );
                p2 = QPointF( r.center().x(), r.bottom() + myYDistance / 2 );
                painter.drawLine( p1, p2 );

                for ( int i = 0; i < this->model()->rowCount(index); ++i ) {
                    QRectF r = itemRect( this->model()->index( i, 0, index ) ).translated( offset.x(), offset.y() );
                    p1 = QPointF( r.center().x(), r.top() );
                    p2 = QPointF( r.center().x(), r.top() - myYDistance / 2 );
                    painter.drawLine( p1, p2 );
                }
            } else if ( rows == 1 ) {
                QPointF p1( r.center().x(), r.bottom() + 1 );
                QPointF p2( p1.x(), p1.y() + myYDistance - 2 );
                painter.drawLine( p1, p2 );
            }
        }
        painter.restore();
}

void VerticalTree::paintConnectionsElbow( QPainter& painter, QModelIndex id ) {
  painter.save();
  painter.setPen( myConnectionPen );
  int rowCount = this->model()->rowCount( id );
  QRectF r = itemRect( id );
  if ( rowCount > 1 ) {
    QRectF child1 = itemRect( this->model()->index( 0, 0, id ) );
    QRectF child2 = itemRect( this->model()->index( rowCount - 1, 0, id ) );

    QPointF p1( child1.center().x(), child1.top() - myYDistance / 2 );
    QPointF p2( child2.center().x(), child2.top() - myYDistance / 2 );
    painter.drawLine( p1, p2 );

    p1 = QPointF( r.center().x(), r.bottom() + 1 );
    p2 = QPointF( r.center().x(), r.bottom() + myYDistance / 2 );
    painter.drawLine( p1, p2 );

    for( int i = 0; i < this->model()->rowCount( id ); ++i ) {
      r = itemRect( this->model()->index( i, 0, id ) );
      p1 = QPointF( r.center().x(), r.top() );
      p2 = QPointF( r.center().x(), r.top() - myYDistance / 2 );
      painter.drawLine( p1, p2 );
    }
  } else if ( rowCount == 1 ) {
    QPointF p1( r.center().x(), r.bottom() + 1 );
    QPointF p2( p1.x(), p1.y() + myYDistance - 2 );
    painter.drawLine( p1, p2 );
  }
  painter.restore();
}

void VerticalTree::paintConnectionsStraight( QPainter& painter, QModelIndex id ) {
  painter.save();
  painter.setPen( myConnectionPen );
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
**        RESOLVING POSITIONS
**
***************************************/

void VerticalTree::positionsInTree() {
    myItemTreePos.clear();
    myDepth = scan( this->model()->index(0,0), QPointF(0,0)).y();
    myLeft = 0;
    //foreach( QPointF p, myItemTreePos.values() ) {
    foreach( QModelIndex idx, myItemTreePos.keys() ) {
      QPointF p = myItemTreePos[idx];
      myLeft = qMax( myLeft, p.x() );
    }
    myDepth -= 1;
    positionsInView();
}

void VerticalTree::positionsInView() {
    myRealSize.setWidth( ( myLeft ) * ( myXDistance + myItemRect.width() ) + myItemRect.width() );
    myRealSize.setHeight( myDepth * ( myYDistance + myItemRect.height() ) + myItemRect.height() );

    setScrollBarValues();

    QPointF offset( myItemRect.width()/2, myItemRect.height()/2 );
    offset += myItemOffset;
    foreach( QModelIndex index, myItemTreePos.keys() ) {
        QPointF p = myItemTreePos[ index ];
        myItemPos[index] = QPointF( p.x() * ( myXDistance + myItemRect.width() ), p.y() * ( myYDistance + myItemRect.height() ) ) + offset;
    }
}

QPointF VerticalTree::scan( QModelIndex index, QPointF leftDepth ) {
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

  qreal left = myItemTreePos[ this->model()->index(0, index.column(), index) ].x();
  qreal right = myItemTreePos[ this->model()->index( rows - 1, index.column(), index ) ].x();
  if ( rows >= 2 ) {
      if ( rows % 2 == 1 ) {
        qreal r = qFloor( rows/2 ) + 1;
        qreal v = myItemTreePos[ this->model()->index( r - 1, index.column(), index ) ].x();
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
  myConnectorType = type;
}

VerticalTree::ConnectorType VerticalTree::connectorType() const {
  return myConnectorType;
}


bool VerticalTree::save( QString filename ) {
    QSize s = myRealSize + QSize( 20, 20 );
    QPixmap pix( s );
    pix.fill( Qt::transparent );
    QPainter painter( &pix );
    painter.setRenderHint( QPainter::Antialiasing );
    qreal x = -myItemOffset.x() + 10;
    qreal y = -myItemOffset.y() + 10;
    paintConnections( painter, QPointF( x, y ) );
    paintItems(painter , QPointF( x, y ) );
    painter.end();
    return pix.save( filename );
}
