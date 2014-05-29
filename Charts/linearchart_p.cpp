#include "linearchart_p.h"
#include "delegates.h"
#include "linearchart.h"
#include "orthogonalaxis.h"
#include "legends/linearchartlegend.h"

#include <qmath.h>

namespace Thistle {

LinearChartPrivate::LinearChartPrivate( LinearChart* q ) : AxisChartPrivate( q ) {

    x = 0;
    pointDelegate = new DotDelegate();

    barDelegate = new BarDelegate();
    orthoAxis = new OrthogonalAxis();
    currentDelegate = 0;

    delete this->legend;
    this->legend = new LinearChartLegend( q );
}

QPair<QPointF, QPointF> LinearChartPrivate::controlPoints( const QPointF& p0, const QPointF& p1, const QPointF& p2, qreal t ) const {
    QPair<QPointF, QPointF> pair;
    qreal d01 = qSqrt( (p1.x() - p0.x() ) * (p1.x() - p0.x() ) +  (p1.y() - p0.y() ) * (p1.y() - p0.y() ) );
    qreal d12 = qSqrt( (p2.x() - p1.x() ) * (p2.x() - p1.x() ) +  (p2.y() - p1.y() ) * (p2.y() - p1.y() ) );

    qreal fa = t * d01 / ( d01 + d12 );
    qreal fb = t * d12 / ( d01 + d12 );

    qreal c1x = p1.x() - fa * ( p2.x() - p0.x() );
    qreal c1y = p1.y() - fa * ( p2.y() - p0.y() );
    qreal c2x = p1.x() + fb * ( p2.x() - p0.x() );
    qreal c2y = p1.y() + fb * ( p2.y() - p0.y() );

    pair.first = QPointF( c1x, c1y );
    pair.second = QPointF( c2x, c2y );

    return pair;
}

void LinearChartPrivate::selectDelegate( Thistle::Types type ) {
    if ( type.testFlag( Thistle::Dot ) ) {
        this->currentDelegate = this->pointDelegate;
    } else if ( type.testFlag( Thistle::Bar ) ) {
        this->currentDelegate = this->barDelegate;
    } else {
        this->currentDelegate = 0;
    }
}

void LinearChartPrivate::paintDelegate( QPainter& painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const {
    if ( this->currentDelegate == 0 ) return;
    this->currentDelegate->paint( &painter, option, index );
}

void LinearChartPrivate::paint( QPainter& painter, int column, Thistle::Types types, bool active ) const {
    const Q_Q( LinearChart );
    painter.save();
    
    if ( types.testFlag( Thistle::Line ) ) this->paintStraightLine( painter, column, active, types.testFlag( Thistle::Area ) );
    else if ( types.testFlag( Thistle::Spline ) ) this->paintSpline( painter, column, active, types.testFlag( Thistle::Area ) );

    if ( types.testFlag( Thistle::Dot ) || types.testFlag( Thistle::Bar ) ) this->paintRaw( painter, column, active );
    painter.restore();
}

void LinearChartPrivate::paintRaw( QPainter& painter, int column, bool isActive ) const {
        const Q_Q( LinearChart );
    int rows = q->model()->rowCount();
    for ( int r = 0; r < rows; ++r ) {
        QModelIndex index = q->model()->index( r, column );
        QStyleOptionViewItem option;
        qreal value = index.data().toReal();
        if ( value < 0 ) {
            option.decorationPosition = QStyleOptionViewItem::Bottom;
        } else {
            option.decorationPosition = QStyleOptionViewItem::Top;
        }
        if ( isActive == false ) {
            option.state = QStyle::State_Off;
        } else {
            option.state = QStyle::State_Selected;
        }
        QRectF rect = q->itemRect( index );
        option.rect = rect.toRect();

        this->paintDelegate( painter, option, index );
    }
}

void LinearChartPrivate::paintStraightLine( QPainter& painter, int column, bool isActive, bool area ) const {
    const Q_Q( LinearChart );
    int rows = q->model()->rowCount();
    QPolygonF polygon;

    QColor penColor = painter.pen().color();
    for ( int r = 0; r < rows; ++r ) {
        QModelIndex index = q->model()->index( r, column );
        
        QRectF rect = q->itemRect( index );
        
        if ( r < (rows - 1 ) ) {
            QRectF r2 = q->itemRect( q->model()->index( r + 1, column ) );

            if ( !isActive ) {
                QPointF p1 = rect.center();
                QPointF p2 = r2.center();
                QLineF line( p1, p2 );
                QLineF l = QLineF( line.pointAt( 0.5 ), line.p2() ).normalVector();
                l.setLength( 4 );
                QLinearGradient gradient( l.p1(), l.p2() );
                QColor c( penColor );
                c.setAlpha( 50 );
                gradient.setColorAt( 0, c );
                gradient.setColorAt( 1, Qt::transparent );
                gradient.setSpread( QLinearGradient::ReflectSpread );
                QPen pen( QBrush( gradient ), 8 );
                painter.setPen( pen );
            }
            if ( area ) polygon << rect.center();
            painter.drawLine( rect.center(), r2.center() );
        }
    }
    if ( area ) {
        QPen pen = painter.pen();
        if ( !isActive ) {
            QBrush brush = painter.brush();
            QColor c( brush.color() );
            c.setAlpha( 25 );
            brush.setColor( c );
            painter.setBrush( brush );
        }
        painter.setPen( Qt::NoPen );
        polygon << q->itemRect( q->model()->index( rows - 1, column ) ).center();
        polygon.prepend( QPointF( polygon.first().x(), this->orthoAxis->origin().y() - 1 ) );
        polygon.append( QPointF( polygon.last().x(), this->orthoAxis->origin().y() - 1 ) );
        painter.drawPolygon( polygon );
        painter.setPen( pen );
        if ( isActive ) {
            polygon.removeFirst();
            polygon.removeLast();
            painter.drawPolyline( polygon );
        }
    }
}

void LinearChartPrivate::paintSpline( QPainter& painter, int column, bool isActive, bool area ) const {
    if ( !area ) {
        painter.setBrush( Qt::NoBrush );
    }
    const Q_Q( LinearChart );
    int rows = q->model()->rowCount();
    if ( rows < 3 ) return;

    QVector<QPointF> points;

    for ( int row = 0; row < rows; ++row ) points << q->itemRect( row, column ).center();
    

    QLineF l( points.at( 1 ), points.at( 0 ) );
    l.setLength( l.length() * 1.5 );
    points.prepend( l.p2() );

    l = QLineF( points.at( points.count() - 2 ), points.at( points.count() - 1) );
    l.setLength( l.length() * 1.5 );
    points << l.p2();

    QPair<QPointF,QPointF> pair = this->controlPoints( points.at( 0 ), points.at( 1 ), points.at( 2 ) );
    QPointF p0 = pair.second;

    QPainterPath path;
    if ( area ) {
        path.moveTo( QPointF( points.at( 1 ).x(), this->orthoAxis->origin().y() ) );
        path.lineTo( points.at( 1 ) );
    } else {
        path.moveTo( points.at( 1 ) );
    }

    for ( int i = 1; i != points.count() - 1; ++i ) {
        QPair<QPointF,QPointF> pair = this->controlPoints( points.at( i - 1 ), points.at( i ), points.at( i + 1 ) );
        path.cubicTo( p0, pair.first, points.at( i ) );
        p0 = pair.second;
    }

    if ( area ) {
        path.lineTo( QPointF( points.last().x(), this->orthoAxis->origin().y() ) );
    }
    painter.drawPath( path );
}

}