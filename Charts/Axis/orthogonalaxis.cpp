#include "orthogonalaxis.h"
#include <QFontMetrics>
#include <QAbstractItemModel>
#include <QPainter>
#include <QDebug>
#include "../../kernel/global.h"

#include "orthogonalaxis_p.h"

namespace Thistle {

OrthogonalAxis::OrthogonalAxis() : Axis( new OrthogonalAxisPrivate() ) {
    Q_D( OrthogonalAxis );
}


void OrthogonalAxis::calculateBounds() {
    Q_D( OrthogonalAxis );
    d->minBound = d->min;
    d->maxBound = d->max;
    if ( d->maxBound == d->minBound ) {
        ++d->maxBound;
        --d->minBound;
    }
    d->order = calculateOrder( d->max - d->min );
    d->tickSize = (d->max - d->min ) / (d->nbTicks - 1);
    if ( d->order >= 10 ) {
        d->nbDigits = 0;
    } else if ( d->order == 1 ) {
        d->nbDigits = 2;
    } else {
        int nbZero = QString::number( d->order ).count( "0" );
        d->nbDigits = nbZero + 2;
    }
}


qreal OrthogonalAxis::calculateOrder( qreal value ) {
    qreal order = 1.0;
    qreal v = qAbs( value );
    if ( v >= 1 ) {
        while ( v > 1 ) {
            order *= 10.0;
            v /= 10.0;
        }
        order /= 10.0;
    } else if ( v != 0 ) {
        while ( v < 1 ) {
            order /= 10.0;
            v *= 10.0;
        }
        order *= 10.0;
    }
    return order;
}


QFont OrthogonalAxis::font() const {
    return QFont();
}



QPointF OrthogonalAxis::origin() const {
    const Q_D( OrthogonalAxis );
    return d->xaxis.p1();
}


void OrthogonalAxis::paintBack( QPainter& painter ) const {
    const Q_D( OrthogonalAxis );
    painter.save();
    painter.setPen( d->axisPen );
    painter.setRenderHint( QPainter::Antialiasing, false );
    this->paintXAxis( painter );
    this->paintYAxis( painter );
    painter.restore();
}


void OrthogonalAxis::paintFront( QPainter& painter ) const {
    const Q_D( OrthogonalAxis );
    painter.save();
    QFontMetrics metrics( d->font );
    int h = metrics.height();
    QPoint textPos( h/2 , this->origin().y() + 5 );
    int n = d->model->rowCount();
    for ( int i = 0; i < n; ++i ) {
        QString s( d->model->headerData( i, Qt::Vertical ).toString() );
        s = metrics.elidedText( s, Qt::ElideRight, d->xLabelsLength  - 3, Qt::TextWrapAnywhere );
        QPointF x = d->xaxis.pointAt( float(i)/float(n) );
        QPoint p1( x.x(), this->origin().y() - 3 );
        painter.save();
        painter.setPen( d->textPen );
        if ( d->verticalLabels == true ) {
            painter.rotate( -90 );
            if ( d->startOnAxis == false ) {
                painter.translate( -textPos.y() - d->xLabelsLength - 3 , p1.x() + this->stepSize()/2.0 );
            } else {
                painter.translate( -textPos.y() - d->xLabelsLength - 3 , p1.x() + h );
            }
            QRect r( 0, 0, d->xLabelsLength, metrics.height() );
            painter.drawText( r, Qt::AlignRight, s );
        } else {
            if ( d->startOnAxis == false ) {
                painter.drawText( p1.x() + this->stepSize()/2.0 - d->xLabelsLength/2.0, textPos.y() + h, s );
            } else {
                painter.drawText( p1.x(), textPos.y() + h, s );
            }
        }
        painter.restore();
    }
    painter.restore();
}


void OrthogonalAxis::paintXAxis( QPainter& painter ) const {
    const Q_D( OrthogonalAxis );
    int n = d->model->rowCount();
    for ( int i = 0; i < n; ++ i ) {
        QPointF x = d->xaxis.pointAt( float(i)/float(n) );
        QLineF l( x - QPoint(0, 3), x + QPoint(0, 3) );
        painter.drawLine( l );
    }
    painter.drawLine( d->xaxis );
}


void OrthogonalAxis::paintYAxis( QPainter& painter ) const {
    /*Paints text on the X & Y axis.*/
    const Q_D( OrthogonalAxis );
    painter.save();
    painter.setPen( d->tickPen );
    QFontMetrics metrics( d->font );
    int h = metrics.height();
    QPoint textPos( h/2 , this->origin().y() + 5 );
    qreal x = this->stepSize() + this->origin().x();
    int i = 0;
    qreal order = d->tickSize;
    int y = order * 2;
    while ( y <= d->maxBound ) {
        QPoint p1( this->origin().x(), this->valueToPoint(y, 0).y() );
        QPoint p2 = p1 + QPoint( d->xaxis.length(), 0 );
        QLineF l( p1, p2 );
        QLineF::IntersectType intersectType;
        QPointF intersectionPoint;
        intersectType = l.intersect( d->xaxis, &intersectionPoint );
        if ( intersectType == QLineF::BoundedIntersection ) {
            l.setP2( intersectionPoint );
        }
        painter.setPen( d->tickPen );
        painter.drawLine( l );
        p1 = QPoint( this->origin().x(), this->valueToPoint( y, 0).y() );
        QString s = QString::number( y, 'f', d->nbDigits );
        QRect r( QPoint( 0, p1.y() - h/2 ), QSize( this->origin().x() - 5 ,h) );
        painter.setPen( d->textPen );
        painter.drawText( r, Qt::AlignRight, s );
        y += order;
    }
    y = order;

    while ( y >= d->minBound ) {
        QPoint p1( this->origin().x(), this->valueToPoint(y, 0).y() );
        if ( qAbs(p1.y() - this->origin().y() ) > 10 ) {
            QPoint p2 = p1 + QPoint( d->xaxis.length(), 0 );
            QLineF l( p1, p2 );
            QLineF::IntersectType intersectType;
            QPointF intersectionPoint;
            intersectType = l.intersect( d->xaxis, &intersectionPoint );
            if ( intersectType == QLineF::BoundedIntersection ) {
                l.setP2( intersectionPoint );
            }
            painter.setPen( d->tickPen );
            painter.drawLine( l );
            p1 = QPoint( this->origin().x(), this->valueToPoint( y, 0).y() );
            QString s = QString::number( y, 'f', d->nbDigits );
            QRect r( QPoint( 0, p1.y() - h/2 ), QSize( this->origin().x() - 5 ,h) );
            painter.setPen( d->textPen );
            painter.drawText( r, Qt::AlignRight, s );
        }
        y -= order;
    }
    painter.restore();
    painter.drawLine( d->yaxis );
}


void OrthogonalAxis::setstartOnAxis( bool startOn ) {
    Q_D( OrthogonalAxis );
    d->startOnAxis = startOn;
}


bool OrthogonalAxis::startOnAxis() const {
    const Q_D( OrthogonalAxis );
    return d->startOnAxis;
}


qreal OrthogonalAxis::stepSize() const {
    const Q_D( OrthogonalAxis );
    return d->xaxis.length() / qreal( d->model->rowCount() );
}


void OrthogonalAxis::update() {
    Q_D( OrthogonalAxis );
    if ( d->model == 0 ) {
        return;
    }
    this->calculateBounds();
    d->yaxis = QLineF( d->valuesRect.bottomLeft() - QPoint( 0, 10 ), d->valuesRect.topLeft() + QPoint( 0, 10 ) );
    QPointF origin = this->valueToPoint( 0, 0 );
    d->xaxis.setP1( QPoint( d->yaxis.p1().x(), origin.y() ) );
    d->xaxis.setP2( d->xaxis.p1() + QPoint( d->valuesRect.width(), 0 ) );
    d->verticalLabels = ( this->stepSize() <= d->xLabelsLength );
    if ( d->verticalLabels == true ) {
        qreal delta = qAbs( d->chartRect.bottom() - origin.y() );
        if ( delta < d->xLabelsLength ) {
            d->valuesRect.setHeight( d->valuesRect.height() - ( d->xLabelsLength - delta ) );
            this->update();
        }
    }
}


QPointF OrthogonalAxis::valueToPoint( qreal value, int axisNumber ) const {
    const Q_D( OrthogonalAxis );
    qreal e = qreal( d->maxBound - d->minBound );
    qreal ratio = ( value - d->minBound) / e;
    qreal y = d->yaxis.pointAt( ratio ).y();
    ratio = qreal(axisNumber+0.5) / qreal(d->model->rowCount());
    qreal x = d->xaxis.pointAt( ratio ).x();
    return QPointF( x, y );
}

}