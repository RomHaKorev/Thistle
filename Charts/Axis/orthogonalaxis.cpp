#include "orthogonalaxis.h"
#include <QFontMetrics>
#include <QAbstractItemModel>
#include <QPainter>
#include "../../Marb.h"


OrthogonalAxis::OrthogonalAxis() : Axis() {
    myModel = 0;
    nbDigits = 3;
    nbTicks = 5;
    tickSize = 0;
    axisPen = QPen( QColor(Marb::LightGray), 1.5 );
    myStartOnAxis = false;
    bool myVerticalLabels = true;
}


void OrthogonalAxis::calculateBounds() {
    minBound = min;
    maxBound = max;
    if ( maxBound == minBound ) {
        ++maxBound;
        --minBound;
    }
    order = calculateOrder( max - min );
    tickSize = (max - min ) / (nbTicks - 1);
    if ( order >= 10 ) {
        nbDigits = 0;
    } else if ( order == 1 ) {
        nbDigits = 2;
    } else {
        int nbZero = QString::number(order).count( "0" );
        nbDigits = nbZero + 2;
    }
}


qreal OrthogonalAxis::calculateOrder( qreal value ) const {
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
    return myXaxis.p1();
}


/*void OrthogonalAxis::paint( QPainter& painter ) {
    painter.save();
    painter.setPen( QPen( Qt::red, 1 ) );
    painter.drawLine( QLineF( myYaxis.p1() + QPoint( 0, 10 ), myYaxis.p2() - QPoint( 0, 10 ) ) );
    painter.drawLine( myXaxis );
    painter.restore();
}*/


void OrthogonalAxis::paintBack( QPainter& painter ) const {
    painter.save();
    painter.setPen( axisPen );
    this->paintXAxis( painter );
    this->paintYAxis( painter );
    painter.restore();
}


void OrthogonalAxis::paintFront( QPainter& painter ) const {
    painter.save();
    QFontMetrics metrics( myFont );
    int h = metrics.height();
    QPoint textPos( h/2 , this->origin().y() + 5 );
    int n = myModel->rowCount();
    for ( int i = 0; i < n; ++i ) {            
        QString s( myModel->headerData( i, Qt::Vertical ).toString() );
        s = metrics.elidedText( s, Qt::ElideRight, xLabelsLength  - 3, Qt::TextWrapAnywhere );
        QPointF x = myXaxis.pointAt( float(i)/float(n) );
        QPoint p1( x.x(), this->origin().y() - 3 );
        painter.save();
        painter.setPen( textPen );
        if ( myVerticalLabels == true ) {
            painter.rotate( -90 );
            if ( myStartOnAxis == false ) {
                painter.translate( -textPos.y() - xLabelsLength - 3 , p1.x() + this->stepSize()/2.0 );
            } else {
                painter.translate( -textPos.y() - xLabelsLength - 3 , p1.x() + h );
            }
            QRect r( 0, 0, xLabelsLength, metrics.height() );
            painter.drawText( r, Qt::AlignRight, s );
        } else {
            if ( myStartOnAxis == false ) {
                painter.drawText( p1.x() + this->stepSize()/2.0 - xLabelsLength/2.0, textPos.y() + h, s );
            } else {
                painter.drawText( p1.x(), textPos.y() + h, s );
            }
        }
        painter.restore();
    }
    painter.restore();
}


void OrthogonalAxis::paintXAxis( QPainter& painter ) const {
    int n = myModel->rowCount();
    for ( int i = 0; i < n; ++ i ) {
        QPointF x = myXaxis.pointAt( float(i)/float(n) );
        QLineF l( x - QPoint(0, 3), x + QPoint(0, 3) );           
        painter.drawLine( l );
    }
    painter.drawLine( myXaxis );
}


void OrthogonalAxis::paintYAxis( QPainter& painter ) const {
    /*Paints text on the X & Y axis.*/
    painter.save();
    painter.setPen(tickPen );
    QFontMetrics metrics( myFont );
    int h = metrics.height();
    QPoint textPos( h/2 , this->origin().y() + 5 );
    qreal x = this->stepSize() + this->origin().x();
    int i = 0;
    qreal order = tickSize;
    int y = order;
    while ( y <= maxBound ) {
        QPoint p1( this->origin().x(), this->valueToPoint(y, 0).y() );
        QPoint p2 = p1 + QPoint( myXaxis.length(), 0 );
        QLineF l( p1, p2 );
        QLineF::IntersectType intersectType;
        QPointF intersectionPoint;
        intersectType = l.intersect( myXaxis, &intersectionPoint );
        if ( intersectType == QLineF::BoundedIntersection ) {
            l.setP2( intersectionPoint );
        }
        painter.setPen( tickPen );
        painter.drawLine( l );
        p1 = QPoint( this->origin().x(), this->valueToPoint( y, 0).y() );
        QString s = QString::number( y, 'f', nbDigits );
        QRect r( QPoint( 0, p1.y() - h/2 ), QSize( this->origin().x() - 5 ,h) );
        painter.setPen( textPen );
        painter.drawText( r, Qt::AlignRight, s );
        y += order;
    }
    y = order;
    while ( y >= minBound ) {
        QPoint p1( this->origin().x(), this->valueToPoint(y, 0).y() );
        QPoint p2 = p1 + QPoint( myXaxis.length(), 0 );
        QLineF l( p1, p2 );
        QLineF::IntersectType intersectType;
        QPointF intersectionPoint;
        intersectType = l.intersect( myXaxis, &intersectionPoint );
        if ( intersectType == QLineF::BoundedIntersection ) {
            l.setP2( intersectionPoint );
        }
        painter.setPen( tickPen );
        painter.drawLine( l );
        p1 = QPoint( this->origin().x(), this->valueToPoint( y, 0).y() );
        QString s = QString::number( y, 'f', nbDigits );
        QRect r( QPoint( 0, p1.y() - h/2 ), QSize( this->origin().x() - 5 ,h) );
        painter.setPen( textPen );
        painter.drawText( r, Qt::AlignRight, s );
        y -= order;
    }
    painter.restore();
    painter.drawLine( myYaxis );
}

void OrthogonalAxis::scanModel() {
    /// Scans values in the model to find the minimum and the maximum. Returns the width needed to display the Y scale.
    /// If the values are greater than zero, the minimum is equal to 0. If the values are less than 0, the maximum is equal to 0.
    /// If a value is not a number (undefined, a string, etc.), she's considered as equal to 0.
    ///
    int rows = myModel->rowCount();
    int cols = myModel->columnCount();
    QFontMetrics metrics( myFont );
    int textWidth = 0;
    qreal min = 0;
    qreal max = 0;
    for( int r = 0; r < rows; ++r ) {
        QString s( myModel->headerData( r, Qt::Vertical ).toString() );
        textWidth = qMax( textWidth, metrics.width( s ) + 5 );
        for( int c = 0; c < cols; ++c ) {
            qreal value = myModel->index( r, c ).data().toReal();
            min = qMin( min, value );
            max = qMax( max, value );
        }
    }
    min = min;
    max = max;
    yLabelsLength = textWidth;
}


void OrthogonalAxis::setModel( QAbstractItemModel* model ) {
    myModel = model;
}


void OrthogonalAxis::setstartOnAxis( bool startOn ) {
    myStartOnAxis = startOn;
}


bool OrthogonalAxis::startOnAxis() const {
    return myStartOnAxis;
}


qreal OrthogonalAxis::stepSize() const {
    return myXaxis.length() / qreal( myModel->rowCount() );
}


void OrthogonalAxis::update() {
    if ( myModel == 0 ) {
        return;
    }
    this->calculateBounds();
    myYaxis = QLineF( valuesRect.bottomLeft() - QPoint( 0, 10 ), valuesRect.topLeft() + QPoint( 0, 10 ) );
    QPointF origin = this->valueToPoint( 0, 0 );
    myXaxis.setP1( QPoint( myYaxis.p1().x(), origin.y() ) );
    myXaxis.setP2( myXaxis.p1() + QPoint( valuesRect.width(), 0 ) );
    xLabelsLength = qMin( xLabelsLength, chartRect.height() * 0.2 );
    myVerticalLabels = ( this->stepSize() <= xLabelsLength );
    if (myVerticalLabels == true) {
        qreal delta = qAbs(chartRect.bottom() - origin.y());
        if ( delta < xLabelsLength ) {
            valuesRect.setHeight( valuesRect.height() - ( xLabelsLength - delta ) );
            this->update();
        }
    }
}


QPointF OrthogonalAxis::valueToPoint( qreal value, int axisNumber ) const {
    qreal e = qreal( maxBound - minBound );
    qreal ratio = ( value - minBound) / e;
    qreal y = myYaxis.pointAt( ratio ).y();
    ratio = qreal(axisNumber+0.5) / qreal(myModel->rowCount());
    qreal x = myXaxis.pointAt( ratio ).x();
    return QPointF( x, y );
}