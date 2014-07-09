#include "graphalgorithm.h"
#include "graphmodel.h"
#include "graphview.h"

#include <qmath.h>
#include <QDebug>

namespace Thistle {

    GraphAlgorithm::GraphAlgorithm( GraphView* parent ) : AbstractGraphAlgorithm( new GraphAlgorithmPrivate() , parent ) {
        Q_D( GraphAlgorithm );
        QObject::connect( d->timer, SIGNAL(timeout()), this, SLOT(processTimer()) );
    }

    void GraphAlgorithm::calculateForces( const QModelIndex& index ) {
        Q_D( GraphAlgorithm );
        Node* node = &( d->itemPos[ index ] );
        GraphView* p = reinterpret_cast<GraphView*>( this->parent() );
        /* Calculate forces pushing the items */

        QPointF velocity( 0, 0 );
        Q_FOREACH ( Node n, d->itemPos.values() ) {
            QPointF vec( node->pos() - n.pos() );
            qreal dx = vec.x();
            qreal dy = vec.y();
            double l = 2.0 * (dx * dx + dy * dy);
            if (l > 0) {
                velocity += QPointF( (dx * 500.0) / l, (dy * 500.0) / l );
            }
        }

        /* Calculate forces pulling items relative to their weight */
        qreal weight = ( node->edges().size() + 1 ) * d->weight;
        Q_FOREACH (Node* n, node->edges()) {
            QPointF vec( node->pos() - n->pos() );
            velocity -= QPointF( vec.x() / weight, vec.y() / weight );
        }
        /* Do not apply velocity if it's too low */
        if (qAbs(velocity.x()) < 0.1 && qAbs(velocity.y()) < 0.1) {
            velocity = QPointF( 0, 0 );
        }
        if ( index != d->movedItem ) {
            node->setPos( node->pos() + velocity );
        }
    }

    void GraphAlgorithm::processTimer() {
        Q_D( GraphAlgorithm );
        GraphView* p = reinterpret_cast<GraphView*>( this->parent() );
        if ( d->itemPos.isEmpty() ) {
            d->movedItem = QModelIndex();
            d->timer->stop();
            return;
        }

        Q_FOREACH( QModelIndex idx, d->itemPos.keys() ) {
            this->calculateForces( idx );
        }

        QPolygonF poly;
        Q_FOREACH ( Node n, d->itemPos.values() ) {
            poly << n.pos();
        }
        QPointF offset = QPointF( 50, 50 ) - poly.boundingRect().topLeft();
        Q_FOREACH ( QModelIndex idx, d->itemPos.keys() ) {
            d->itemPos[ idx ].setPos( d->itemPos[ idx ].pos() + offset );
        }
        d->realSize = poly.boundingRect().size() + 2 * ( p->itemRect( p->model()->index( 0, 0 ) ).size() );
        
        emit updated();

        qreal length = 0;
        QList<QPointF> pts;
        Q_FOREACH( Node n, d->itemPos.values() ) {
            pts << n.pos();
        }
        QPointF p1 = pts[0];
        for ( int i = 1; i < pts.size(); ++i ) {
            length += QLineF( p1, pts[i] ).length();
            p1 = pts[i];
        }
        if ( qAbs(length - d->oldLength ) > 0.0001 ) {
            d->oldLength = length;
        } else {
            d->timer->stop();
        }
    }


    void GraphAlgorithm::run() {
        Q_D( GraphAlgorithm );
        d->timer->stop();
        GraphView* p = reinterpret_cast<GraphView*>( this->parent() );
        if ( p == 0 || p->model() == 0 ) {
            return;
        }
        d->itemPos.clear();
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
                d->itemPos.insert( idx, n );
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
            d->itemPos[ e.leftIndex ].addEdge( &( d->itemPos[ e.rightIndex ] ) );
            d->itemPos[ e.rightIndex ].addEdge( &( d->itemPos[ e.leftIndex ] ) );
        }
        d->timer->start();
        d->oldLength = 0;
    }

    void GraphAlgorithm::setNodeWeight( qreal weight ) {
        Q_D( GraphAlgorithm );
        d->weight = weight;
    }

}
