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
#include "radialtree.h"
#include <QPaintEvent>
#include <QPainter>
#include <QAbstractItemModel>
#include <QFontMetrics>
#include <QScrollBar>
#include <qmath.h>
#include "../kernel/marbitemdelegate.h"


RadialTree::RadialTree( QWidget* parent ) : Tree( parent ) {
    //myRotateText = false;
    //myDisplayCircle = true;
    myItemRect = QRect( -20, -20, 40, 40 );
    MarbStyle style = MarbStyle();
    style.setShape( Marb::Ellipse );
    myDelegate = new MarbItemDelegate( this );
    myDelegate->setItemStyle( style );
    setItemDelegate( myDelegate );
    myYDistance = 40;
    myXDistance = 40;
    myItemOffset = QPoint( 0, 0 );
    myRadius = 1;
    myDiagonal = 1;
}


void RadialTree::paintCircles( QPainter& painter, QPointF offset ) {
    painter.save();
    QColor c = myConnectionPen.color();
    c.setAlpha( c.alpha() * 0.15 );
    QPen pen( myConnectionPen );
    pen.setColor( c );
    pen.setWidth( pen.width() * 2 );
    painter.setPen( pen );
    painter.setRenderHint( QPainter::Antialiasing );
    QModelIndex index = this->model()->index(0,0);
    QPointF center = itemRect(index).translated( offset.x(), offset.y() ).center();
    for( int rd = 1; rd < myDepth; ++rd ) {
            qreal r = myRadius * rd / myDepth;
            painter.drawEllipse( center, r, r );
    }
    painter.restore();
}


void RadialTree::paintConnections( QPainter& painter, QPointF offset ) {
                paintCircles( painter, offset );
                Tree::paintConnections( painter, offset );
}


void RadialTree::positionsInTree() {
    myItemTreePos.clear();
    myOrderedIndexes.clear();
    QPointF p = scan( this->model()->index(0,0) , QPointF(0,0));
    myLeft = p.x() + 1;
    myDepth = p.y();
    positionsInView();
}


void RadialTree::positionsInView() {
	if ( myItemTreePos.isEmpty() || (myRadius == HUGE_VAL) || (myRadius == 0) ) {
            return;
    }
    updatePerimeter();
    if ( (myRadius == HUGE_VAL) || (myRadius == 0) ) {
        return;
    }
    myRealSize.setWidth( myRadius * 2 + myItemRect.width());
    myRealSize.setHeight( myRadius * 2 + myItemRect.height() );
    setScrollBarValues();
    myItemPos = myItemTreePos;
    QPainterPath path;
    path.addEllipse( myItemOffset + myItemRect.center(), myRadius, myRadius );
    qreal l = 0;
    qreal factor = 0;
    Q_FOREACH( QModelIndex index, myOrderedIndexes ) {
        if ( this->model()->rowCount( index ) != 0 ) {
                    continue;
        }
        factor = qreal(myDepth) / qreal(myItemPos[index].y());
        if ( this->model()->rowCount( index ) == 0 ) {
                l +=	(qreal(myDiagonal)/2.0) * qreal(factor);
        }
        qreal percent = path.percentAtLength( l );
        myItemPos[index] = QPointF( percent, myItemPos[index].y() );
        if ( this->model()->rowCount( index ) == 0 ) {
                l +=	(qreal(myDiagonal)/2.0) * qreal(factor);
        }
    }
    QList<QModelIndex> reversedOrderedIndexes;
    Q_FOREACH ( QModelIndex idx, myOrderedIndexes ) {
        reversedOrderedIndexes.prepend( idx );
    }
    Q_FOREACH ( QModelIndex index, reversedOrderedIndexes ) {
            if ( this->model()->rowCount( index ) == 0 ) {
                    continue;
            }
            QModelIndex left = this->model()->index( 0,0, index );
            QModelIndex right = this->model()->index( this->model()->rowCount(index) - 1,0, index );
            myItemPos[index] = QPointF( float(myItemPos[left].x() + myItemPos[right].x()) / 2.0, myItemPos[index].y() );
    }
    myItemPos[model()->index(0,0)] = QPointF(0, 0);
    Q_FOREACH ( QModelIndex index, myOrderedIndexes ) {
        if ( myDepth == myItemPos[index].y() ) {
                factor = 1.0;
        } else {
                if ( myItemPos[index].y() != 0 ) {
                        factor = qreal(myDepth) / qreal(myItemPos[index].y());
                } else {
                        factor = 1.0;
                }
        }
        qreal radius = qreal( myRadius ) / qreal(factor);
        QPainterPath circle;
        circle.addEllipse( myItemRect.center(), radius, radius );
        myItemPos[index] = circle.pointAtPercent( myItemPos[index].x() ) + myItemOffset;
    }
    myItemPos[ this->model()->index(0,0) ] = myItemOffset;
}


bool RadialTree::save( QString filename ) {
        QSize s = myRealSize + QSize( 20, 20 );
        QPixmap pix( s );
        pix.fill( Qt::transparent );
        QPainter painter( &pix );
        painter.setRenderHint( QPainter::Antialiasing );
        qreal x = myRealSize.width()/2 + 10 - myItemOffset.x();
        qreal y = myRealSize.height()/2 + 10 - myItemOffset.y();
        paintConnections( painter, QPointF( x, y ) );
        paintItems( painter , QPointF( x, y ) );
        painter.end();
        return pix.save( filename );
}


QPointF RadialTree::scan(QModelIndex index, QPointF leftDepth) {
    myOrderedIndexes.append( index );
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
    qreal left = myItemTreePos[ this->model()->index(0, 0, index) ].x();
    qreal right =    myItemTreePos[ this->model()->index( rows - 1, 0, index ) ].x();
    if ( rows >= 2 ) {
            if ( rows % 2 == 1 ) {
                    qreal r = qFloor( rows/2 ) + 1;
                    qreal v = myItemTreePos[ this->model()->index( r - 1, 0, index ) ].x();
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
    my_rotateText = rotate;
}
/***************************************
**
**                RESOLVING POSITIONS
**
***************************************/


void RadialTree::setScrollBarValues() {
    QSize s = myRealSize + QSize(50, 50);
    qreal dw = qMax( 0, ( s.width() - width() )/2	);
    qreal dh = qMax ( 0, ( s.height() - height() )/2 );
//    qreal dw = qMax( 0, ( myRealSize.width() - width() )/2 + verticalScrollBar()->width() + 50 );
//    qreal dh = qMax ( 0, ( myRealSize.height() - height() )/2 + horizontalScrollBar()->height() + 50 );
    horizontalScrollBar()->setRange( -dw, dw );
    verticalScrollBar()->setRange( -dh, dh );
    myItemOffset = QPoint( width() / 2, height() / 2 );
}


void RadialTree::updatePerimeter() {
    qreal w = myItemRect.width() + myXDistance;
    qreal h = myItemRect.height() + myYDistance;
    myDiagonal = qSqrt( w * w + h * h );
    myPerimeter = 0;
    qreal factor = 1;
    Q_FOREACH ( QModelIndex index, myItemTreePos.keys() ) {
        if ( this->model()->rowCount( index ) == 0 ) {
            factor = ( myDepth ) / ( myItemTreePos.value( index ).y() );
            myPerimeter += myDiagonal * factor;
        }
    }
    myRadius = myPerimeter / ( 2 * M_PI );
    if ( myRadius*2 < myDiagonal * myDepth ) {
        myRadius = ( myDiagonal * myDepth )/2 * 1.5;
    }
}


