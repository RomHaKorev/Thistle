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
#include "graph.h"
#include <QScrollBar>

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <qmath.h>

#include "graphmodel.h"

#include "../kernel/marbitemdelegate.h"
static const qreal Pi = 3.14159265358979323846264338327950288419717;


Graph::Graph(QWidget *parent) : MarbAbstractItemView(parent), myTimer(this) {
    myTimer.setInterval(1);
    setDragEnabled( true );
    connect( &myTimer, SIGNAL(timeout()), this, SLOT(processTimer()) );
    MarbItemDelegate* delegate = new MarbItemDelegate( this );
    MarbStyle style = delegate->itemStyle();
    style.setShape( Marb::Ellipse );
    delegate->setItemStyle( style );
    setItemDelegate( delegate );
    myMovableItem = true;
    myElasticItem = true;
    myWeight = 9.81;
    myItemOffset = QPointF( 0, 0 );

    myModel = 0;
}


void Graph::calculateForces( QModelIndex& idx ) {
    Node* node = &( myItemPos[ idx ] );
    /* Calculate forces pushing the items */
    QPointF velocity( 0, 0 );
    Q_FOREACH ( Node n, myItemPos.values() ) {
        QPointF vec( node->pos() - n.pos() );
        qreal dx = vec.x();
        qreal dy = vec.y();
            double l = 2.0 * (dx * dx + dy * dy);
            if (l > 0) {
                    velocity += QPointF( (dx * 500.0) / l, (dy * 500.0) / l );
            }
    }
    /* Calculate forces pulling items relative to their weight */
    qreal weight = ( node->edges().size() + 1 ) * myWeight;
    Q_FOREACH (Node* n, node->edges()) {
            QPointF vec( node->pos() - n->pos() );
            velocity -= QPointF( vec.x() / weight, vec.y() / weight );
    }
    /* Do not apply velocity if it's too low */
    if (qAbs(velocity.x()) < 0.1 && qAbs(velocity.y()) < 0.1) {
        velocity = QPointF( 0, 0 );
    }
    if ( idx != myMovedItem ) {
        node->setPos( node->pos() + velocity );
    }
}


QModelIndex Graph::indexAt(const QPoint &point) const {
    QPoint p = point    + QPoint( horizontalOffset(), verticalOffset() );
    if ( this->model() == 0 ) {
        return QModelIndex();
    }
    Q_FOREACH( QModelIndex index, myItemPos.keys() ) {
            QPainterPath path = this->itemPath( index );
            if ( path.contains( p ) ) {
                return index;
            }
    }
    return QModelIndex();
}


QPainterPath Graph::itemPath(const QModelIndex &index) const {
    QPainterPath path;
    if ( !myItemPos.contains( index ) ) {
        return path;
    }
    QPointF pos = myItemPos.value( index ).pos() - QPointF( horizontalOffset(), verticalOffset() );
    path.addRect( QRect(-20,-20,40,40).translated( pos.x(), pos.y() ) );
    return path;
}


void Graph::mouseMoveEvent( QMouseEvent* event ) {
    QPoint p = QPoint( this->horizontalOffset(), this->verticalOffset() );
    if ( myMovableItem == true && myMovedItem.isValid() ) {
        if ( !myDragDropTime.isNull() ) {
            QTime current = QTime::currentTime();
            if ( myDragDropTime.msecsTo( current ) > 50 ) {
                myItemPos[ myMovedItem ].setPos( event->pos() + p );
            }
        }
    }
    MarbAbstractItemView::mouseMoveEvent( event );
}


void Graph::mousePressEvent( QMouseEvent* event ) {
    if ( myMovableItem == true ) {
        QModelIndex idx = this->indexAt( event->pos() );
        if ( idx.isValid() ) {
            myMovedItem = idx;
            myDragDropTime = QTime::currentTime();
        }
    }
    //MarbAbstractItemView::mousePressEvent( event );
}


void Graph::mouseReleaseEvent( QMouseEvent* event ) {
    //MarbAbstractItemView::mouseReleaseEvent( event );
    if ( myMovableItem == true ) {
        if ( myElasticItem == true ) {
            myTimer.start();
        } else {
            myMovedItem = QModelIndex();
        }
        myDragDropTime = QTime();
    }
}


void Graph::paintArrow( QPainter& painter, QLineF line ) {
    QPen originalPen = painter.pen();
    QPen pen = painter.pen();
    pen.setWidth( 1 );
    painter.setPen( pen );
    QPointF p1 = line.p2();
    QLineF l( p1, line.pointAt( (line.length() - 15)/line.length() ) );
    l.setAngle( l.angle() + 30 );
    QPointF p2 = l.p2();
    l = QLineF( p1, line.pointAt( (line.length() - 15)/line.length() ) );
    l.setAngle( l.angle() - 30 );
    QPointF p3 = l.p2();
    painter.drawPolygon( QPolygonF() << p1 << p2 << p3 );
    painter.setPen( originalPen );
}


void Graph::paintEdge( QPainter& painter, QModelIndex idx1, QModelIndex idx2, Edge::Type type ) {
    QRectF r1 = itemRect( idx1 );
    QRectF r2 = itemRect( idx2 );
    QPointF p1 = r1.center();
    QPointF p2 = r2.center();
    QLineF line( p1, p2    );
    if ( type == Edge::NoArrow ) {
        painter.drawLine( line );
        return;
    }
    int i = 0;
    QList<QPointF> l;
    QPointF p;
    l << r1.topLeft() << r1.topRight() << r1.bottomRight() << r1.bottomLeft() << r1.topLeft();
    while ( i < ( l.size() - 1 ) ) {
        if ( line.intersect( QLineF( l[i], l[i + 1] ), &p ) == QLineF::BoundedIntersection ) {
            p1 = p;
            break;
        }
        ++i;
    }
    i = 0;
    l.clear();
    l << r2.topLeft() << r2.topRight() << r2.bottomRight() << r2.bottomLeft() << r2.topLeft();
    while ( i < ( l.size() - 1 ) ) {
        if ( line.intersect( QLineF( l[i], l[i + 1] ), &p ) == QLineF::BoundedIntersection ) {
            p2 = p;
            break;
        }
        ++i;
    }
    line = QLineF( p1, p2    );
    if ( type == Edge::Bilateral ) {
        painter.drawLine( line.pointAt( 0.1 ), line.pointAt( 0.9 ) );
        paintArrow( painter, QLineF( p1, p2 ) );
        paintArrow( painter, QLineF( p2, p1 ) );
    } else {
        painter.drawLine( line.p1(), line.pointAt( 0.9 ) );
        paintArrow( painter, QLineF( p1, p2 ) );
    }
}


void Graph::paintEdges( QPainter& painter, QPointF offset ) {
    Q_UNUSED( offset )
    painter.save();
    painter.setPen( QPen( QColor( Marb::Gray ), 3 ) );
    painter.setBrush( QColor( Marb::Gray ) );
    Q_FOREACH( Edge edge, myModel->edges() ) {
        paintEdge( painter, edge.leftIndex,    edge.rightIndex, edge.type );
    }
    painter.restore();
}


void Graph::paintEvent( QPaintEvent* /*event*/ ) {
    if ( myModel == 0 ) {
        return;
    }
    QPainter painter(viewport());
    painter.setRenderHint( QPainter::Antialiasing );
    paintEdges( painter );
    paintItems( painter );
}


void Graph::paintItems( QPainter& painter, QPointF offset ) {
    Q_FOREACH( QModelIndex idx, myItemPos.keys()    ) {
        QStyleOptionViewItem option;
        option.rect = itemRect( idx ).translated( offset.x(), offset.y() ).toRect();
        itemDelegate()->paint( &painter, option, idx );
    }
}


void Graph::processTimer() {
    if ( myItemPos.isEmpty() ) {
        myMovedItem = QModelIndex();
        myTimer.stop();
        return;
    }
    Q_FOREACH( QModelIndex idx, myItemPos.keys() ) {
        calculateForces( idx );
        calculateForces( idx );
    }
    QPolygonF poly;
    Q_FOREACH ( Node n, myItemPos.values() ) {
        poly << n.pos();
    }
    QPointF offset = QPointF( 50, 50 ) - poly.boundingRect().topLeft();
    Q_FOREACH ( QModelIndex idx, myItemPos.keys() ) {
        myItemPos[ idx ].setPos( myItemPos[ idx ].pos() + offset );
    }
    myRealSize = poly.boundingRect().size() + 2 * ( itemRect( this->model()->index( 0, 0 ) ).size() );
    update();
    viewport()->update();
    qreal length = 0;
    QList<QPointF> pts;
    Q_FOREACH( Node n, myItemPos.values() ) {
        pts << n.pos();
    }
    QPointF p1 = pts[0];
    for ( int i = 1; i < pts.size(); ++i ) {
        length += QLineF( p1, pts[i] ).length();
        p1 = pts[i];
    }
    if ( qAbs(length - oldLength ) > 0.0001 ) {
        oldLength = length;
    } else {
        myTimer.stop();
        //setScrollBarValues();
    }
}


bool Graph::save( QString filename ) {
    QSizeF s = myRealSize + QSize( 20, 20 );
    QPixmap pix( s.toSize() );
    pix.fill( Qt::transparent );
    QPainter painter( &pix );
    painter.setRenderHint( QPainter::Antialiasing );
    paintEdges( painter );
    paintItems(painter );
    painter.end();
    return pix.save( filename );
}


void Graph::setScrollBarValues() {
    qreal dw = qMax( 0.0, myRealSize.width() - width()	);
    qreal dh = qMax ( 0.0, myRealSize.height() - height() );
    horizontalScrollBar()->setRange( 0, dw );
    verticalScrollBar()->setRange( 0, dh );
    myItemOffset = QPoint( 0, 0 );
}

void Graph::setModel( QAbstractItemModel* model ) {
    GraphModel* m = qobject_cast<GraphModel*>( model );
    if ( m != 0 ) {
        myModel = m;
        MarbAbstractItemView::setModel( m );
    } else {
        qWarning() << tr("Model must be an instance of GraphModel.");
    }
}

void Graph::updateValues() {
    if ( myModel == 0 ) {
        return;
    }
    myItemPos.clear();
    int rows = myModel->rowCount();
    int nbRows = qFloor( qSqrt( rows ) );
    int x = 10;
    int y = 10;
    int i = 0;
    for( int r = 0; r < rows; ++ r ) {
        QModelIndex idx = myModel->index( r, 0 );
        if ( idx.isValid() && idx.data().isValid() ) {
            Node n;
            n.setPos( QPointF( x, y ) );
            //n.setPos(10 + qrand() % 200, 10 + qrand() % 200);
            myItemPos.insert( idx, n );
            x += 100;
            ++i;
            if ( i > nbRows ) {
                x = 10;
                y += 100;
                i = 0;
            }
        }
    }
    Q_FOREACH ( Edge e, myModel->edges() ) {
        myItemPos[ e.leftIndex ].addEdge( &(myItemPos[ e.rightIndex ]) );
        myItemPos[ e.rightIndex ].addEdge( &(myItemPos[ e.leftIndex ]) );
    }
    myTimer.start();
    oldLength = 0;
}
