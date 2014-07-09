#include "edge.h"

#include <QDebug>

namespace Thistle {

    bool operator==( Edge& e1, Edge& e2 ) {
        if ( e1.leftIndex == e2.leftIndex && e1.rightIndex == e2.rightIndex ) {
            return true;
        }
        return false;
    }

    Edge::Edge( QModelIndex idx1, QModelIndex idx2, Edge::Type t ) {
        leftIndex = idx1;
        rightIndex = idx2;
        type = t;
    }

    void Edge::paintArrow( QPainter& painter, const QLineF& line ) const {
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
        painter.drawPolygon( QPolygon() << p1.toPoint() << p2.toPoint() << p3.toPoint() );
        painter.setPen( originalPen );
    }


    void Edge::paintEdge( QPainter& painter, QRectF r1, QRectF r2 ) const {
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
            this->paintArrow( painter, QLineF( p1, p2 ) );
            this->paintArrow( painter, QLineF( p2, p1 ) );
        } else {
            painter.drawLine( line.p1(), line.pointAt( 0.9 ) );
            this->paintArrow( painter, QLineF( p1, p2 ) );
        }
    }

}