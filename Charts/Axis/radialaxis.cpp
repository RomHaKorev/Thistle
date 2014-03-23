#include "RadialAxis.h"
#include "../../kernel/Marb.h"

#include <QAbstractItemModel>
#include <QDebug>

RadialAxis::RadialAxis() {
    myCenterHoleDiam = 0;
    myOrigin = QPointF(0,0);
    nbTicks = 5;
}


RadialAxis::~RadialAxis() {
}


qreal RadialAxis::centerHoleDiam() const {
    return myCenterHoleDiam;
}


QPointF RadialAxis::origin() const {
    return valuesRect.center();
}


void RadialAxis::update() {
    this->calculateBounds();
    QFontMetrics metrics( this->font() );
    int h = metrics.height();
    valuesRect.setWidth( valuesRect.width() - h * 2 );
    valuesRect.setHeight( valuesRect.height() - h * 2 );
    valuesRect.translate( h, h );
    myCenterHoleDiam = valuesRect.width()/2 * 0.2;
    qreal w = valuesRect.width()/2.0;
    yAxis = QLineF( valuesRect.center() + QPoint( myCenterHoleDiam, 0 ), valuesRect.center() + QPoint( w, 0 ) );
}


QPointF RadialAxis::valueToPoint( qreal value, int axisNumber ) const {
    QPainterPath p;
    p.addEllipse( this->valueToRect( value ) );
    return p.pointAtPercent( axisNumber / ( this->model()->rowCount()) );
}


QRect RadialAxis::valueToRect( qreal value ) const {
    float e = maxBound - minBound;
    qreal ratio = abs( float( value - minBound) / e );
    int w = yAxis.pointAt( ratio ).x() - yAxis.p1().x() + myCenterHoleDiam;
    return QRect( this->origin().x() - w, this->origin().y() - w, w*2, w*2 );
}


void RadialAxis::paintBack( QPainter& painter ) const {
    qreal y = minBound;
    painter.save();

    painter.setPen( axisPen );

    QFontMetrics metrics( this->font() );
    int h = metrics.height();
    painter.drawLine( valuesRect.center(), QPointF( valuesRect.center().x(), valuesRect.top() - 10 ) ); 

    painter.setPen( tickPen );

    while ( y <= maxBound ) {
        QString text = QString::number( y );
        int w = metrics.width( text );
        QRect rectangle = this->valueToRect( y );
        QPainterPath path;
        path.addEllipse( rectangle );
        painter.drawPath( path );

        painter.eraseRect( valuesRect.center().x() - w/2.0, rectangle.y() - h - 1, w, h );

        y += tickSize;
    }

    this->paintText( painter );
    
    painter.restore();
}


void RadialAxis::paintFront( QPainter& painter ) const {
    painter.save();
    painter.setPen( QPen( QColor( Marb::DarkGray ), 1.5 ) );
    QFontMetrics metrics( this->font() );
    qreal v = minBound;
    while ( v <= maxBound ) {
        qreal y = this->valueToRect( v ).top();
        QString text = QString::number( v );
        int w = metrics.width( text );
        painter.drawText( valuesRect.center().x() - w/2.0, y - 2, text );
        v += tickSize;
    }
    painter.restore();
}

void RadialAxis::paintText( QPainter& painter )  const {
    qreal angle = this->stepSize();
    qreal delta = angle * 0.1;
    int rows = this->model()->rowCount();

    QFontMetrics metrics( this->font() );
    int h = metrics.height();

    QPainterPath path;
    QRect r( valuesRect );
    r.translate( -13, -13 );
    r.setWidth( r.width() + 26 );
    r.setHeight( r.height() + 26 );
    path.addEllipse( r );

    QPainterPath boundingPath;
    boundingPath.addEllipse( this->valuesRect );

    for ( int i = 0; i < rows; ++i ) {
        QString s( this->model()->headerData( i, Qt::Vertical ).toString() );
        QRect textRect;

        qreal start = (rows - 1 - i) * this->stepSize() + this->startAngle();
        qreal ratio = ( start + angle/2.0 ) / 360.0 + 0.5;
        if ( ratio > 1 ) ratio -=1;
        QPointF p = path.pointAtPercent( ratio );
        QPointF p1 = boundingPath.pointAtPercent( ratio );

        painter.save();
        painter.setPen( Qt::NoPen );
        painter.setBrush( QColor( 150, 150, 150, 30 ) );
        painter.drawPie( this->valuesRect, (start+delta/2) * 16, (angle-delta) * 16 );
        painter.restore();

        if ( p.y() <= p1.y() ) { // To the top
            textRect.setY( p.y() - h );
        } else {                 // To the bottom
            textRect.setY( p.y() );
        }

        textRect.setHeight( h );

        if ( p.x() >= p1.x() ) { // To the right
            textRect.setX( p.x() );
            textRect.setWidth( chartRect.right() - p.x() );
            painter.drawText( textRect, Qt::AlignLeft, s );

        } else {                // To the left
            textRect.setX( chartRect.left() );
            textRect.setWidth( p.x() - chartRect.left() );
            painter.drawText( textRect, Qt::AlignRight, s );
        }
    }
}

qreal RadialAxis::startAngle() const {
    return (90 + 10);
}


qreal RadialAxis::stepSize() const {
    return (360.0 - 20) / qreal( myModel->rowCount() );
}

