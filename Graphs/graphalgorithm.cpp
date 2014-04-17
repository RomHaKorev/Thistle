#include "graphalgorithm.h"
#include "graphmodel.h"
#include "graph.h"

#include <qmath.h>
#include <QDebug>

namespace Marb {

GraphAlgorithm::GraphAlgorithm( Graph* parent ) : QObject( parent ) {
    d_ptr = new GraphAlgorithmPrivate();
    QObject::connect( d_ptr->timer, SIGNAL(timeout()), this, SLOT(processTimer()) );
}

void GraphAlgorithm::calculateForces( const QModelIndex& index ) {
    Node* node = &( d_ptr->itemPos[ index ] );
    Graph* p = reinterpret_cast<Graph*>( this->parent() );
    /* Calculate forces pushing the items */
    
    QPointF velocity( 0, 0 );
    Q_FOREACH ( Node n, d_ptr->itemPos.values() ) {
        QPointF vec( node->pos() - n.pos() );
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
                velocity += QPointF( (dx * 500.0) / l, (dy * 500.0) / l );
        }
    }

    /* Calculate forces pulling items relative to their weight */
    qreal weight = ( node->edges().size() + 1 ) * d_ptr->weight;
    Q_FOREACH (Node* n, node->edges()) {
            QPointF vec( node->pos() - n->pos() );
            velocity -= QPointF( vec.x() / weight, vec.y() / weight );
    }
    /* Do not apply velocity if it's too low */
    if (qAbs(velocity.x()) < 0.1 && qAbs(velocity.y()) < 0.1) {
        velocity = QPointF( 0, 0 );
    }
    if ( index != d_ptr->movedItem ) {
        node->setPos( node->pos() + velocity );
    }
}


const Node GraphAlgorithm::node( const QModelIndex& index ) const {
    return d_ptr->itemPos.value( index, Node() );
}


void GraphAlgorithm::processTimer() {
    Graph* p = reinterpret_cast<Graph*>( this->parent() );
    if ( d_ptr->itemPos.isEmpty() ) {
        d_ptr->movedItem = QModelIndex();
        d_ptr->timer->stop();
        return;
    }

    Q_FOREACH( QModelIndex idx, d_ptr->itemPos.keys() ) {
        this->calculateForces( idx );
        //this->calculateForces( idx );
    }

    QPolygonF poly;
    Q_FOREACH ( Node n, d_ptr->itemPos.values() ) {
        poly << n.pos();
    }
    QPointF offset = QPointF( 50, 50 ) - poly.boundingRect().topLeft();
    Q_FOREACH ( QModelIndex idx, d_ptr->itemPos.keys() ) {
        d_ptr->itemPos[ idx ].setPos( d_ptr->itemPos[ idx ].pos() + offset );
    }
    d_ptr->realSize = poly.boundingRect().size() + 2 * ( p->itemRect( p->model()->index( 0, 0 ) ).size() );
    emit update();
    qreal length = 0;
    QList<QPointF> pts;
    Q_FOREACH( Node n, d_ptr->itemPos.values() ) {
        pts << n.pos();
    }
    QPointF p1 = pts[0];
    for ( int i = 1; i < pts.size(); ++i ) {
        length += QLineF( p1, pts[i] ).length();
        p1 = pts[i];
    }
    if ( qAbs(length - d_ptr->oldLength ) > 0.0001 ) {
        d_ptr->oldLength = length;
    } else {
        d_ptr->timer->stop();
    }
}


void GraphAlgorithm::run() {
    Graph* p = reinterpret_cast<Graph*>( this->parent() );
    if ( p->model() == 0 ) {
        return;
    }
    d_ptr->itemPos.clear();
    int rows = p->model()->rowCount();
    int nbRows = qFloor( qSqrt( rows ) );
    int x = 10;
    int y = 10;
    int i = 0;
    for( int r = 0; r < rows; ++r ) {
        QModelIndex idx = p->model()->index( r, 0 );
        if ( idx.isValid() && idx.data().isValid() ) {
            Node n;
            n.setPos( QPointF( x, y ) );
            //n.setPos(10 + qrand() % 200, 10 + qrand() % 200);
            d_ptr->itemPos.insert( idx, n );
            x += 100;
            ++i;
            if ( i > nbRows ) {
                x = 10;
                y += 100;
                i = 0;
            }
        }
    }
    GraphModel* m = qobject_cast<GraphModel*>( p->model() );
    Q_FOREACH ( Edge e, m->edges() ) {
        d_ptr->itemPos[ e.leftIndex ].addEdge( &( d_ptr->itemPos[ e.rightIndex ] ) );
        d_ptr->itemPos[ e.rightIndex ].addEdge( &( d_ptr->itemPos[ e.leftIndex ] ) );
    }
    d_ptr->timer->start();
    d_ptr->oldLength = 0;
}

}
