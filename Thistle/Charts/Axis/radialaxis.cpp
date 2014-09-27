#include "RadialAxis.h"
#include "../../kernel/global.h"

#include <QAbstractItemModel>
#include <QDebug>

#include "radialaxis_p.h"

namespace Thistle
{

RadialAxis::RadialAxis() : AbstractAxis( new RadialAxisPrivate() )
{}


RadialAxis::~RadialAxis()
{}


qreal RadialAxis::centerHoleDiam() const
{
    const Q_D( RadialAxis );
    return d->centerHoleDiam;
}


QPointF RadialAxis::origin() const
{
    const Q_D( RadialAxis );
    return d->valuesRect.center();
}


void RadialAxis::update()
{
    Q_D( RadialAxis );
    this->calculateBounds();
    QFontMetrics metrics( this->font() );
    int h = metrics.height();
    d->valuesRect.setWidth( d->valuesRect.width() - h * 2 );
    d->valuesRect.setHeight( d->valuesRect.height() - h * 2 );
    d->valuesRect.translate( h, h );
    d->centerHoleDiam = d->valuesRect.width()/2 * 0.2;
    qreal w = d->valuesRect.width()/2.0;
    d->yaxis = QLineF( d->valuesRect.center() + QPoint( d->centerHoleDiam, 0 ), d->valuesRect.center() + QPoint( w, 0 ) );
}


QPointF RadialAxis::valueToPoint( qreal value, int axisNumber ) const
{
    QPainterPath p;
    p.addEllipse( this->valueToRect( value ) );
    return p.pointAtPercent( axisNumber / ( this->model()->rowCount()) );
}


QRect RadialAxis::valueToRect( qreal value ) const
{
    const Q_D( RadialAxis );
    float e = d->maxBound - d->minBound;
    qreal ratio = abs( float( value - d->minBound) / e );
    int w = d->yaxis.pointAt( ratio ).x() - d->yaxis.p1().x() +d->centerHoleDiam;
    return QRect( this->origin().x() - w, this->origin().y() - w, w*2, w*2 );
}


void RadialAxis::paintBack( QPainter& painter ) const
{
    const Q_D( RadialAxis );
    qreal y = d->minBound;
    painter.save();

    painter.setPen( d->axisPen );

    QFontMetrics metrics( this->font() );
    int h = metrics.height();
    painter.drawLine( d->valuesRect.center(), QPointF( d->valuesRect.center().x(), d->valuesRect.top() - 10 ) );

    painter.setPen( d->tickPen );

    while ( y <= d->maxBound )
    {
        QString text = QString::number( y );
        int w = metrics.width( text );
        QRect rectangle = this->valueToRect( y );
        QPainterPath path;
        path.addEllipse( rectangle );
        painter.drawPath( path );

        painter.eraseRect( d->valuesRect.center().x() - w/2.0, rectangle.y() - h - 1, w, h );

        y += d->tickSize;
    }

    this->paintText( painter );

    painter.restore();
}


void RadialAxis::paintFront( QPainter& painter ) const
{
    const Q_D( RadialAxis );
    painter.save();
    painter.setPen( QPen( QColor( Thistle::Colors::DarkGray ), 1.5 ) );
    QFontMetrics metrics( this->font() );
    qreal v = d->minBound;
    while ( v <= d->maxBound )
    {
        qreal y = this->valueToRect( v ).top();
        QString text = QString::number( v );
        int w = metrics.width( text );
        painter.drawText( d->valuesRect.center().x() - w/2.0, y - 2, text );
        v += d->tickSize;
    }
    painter.restore();
}

void RadialAxis::paintText( QPainter& painter )  const
{
    const Q_D( RadialAxis );
    qreal angle = this->stepSize();
    qreal delta = angle * 0.1;
    int rows = this->model()->rowCount();

    QFontMetrics metrics( this->font() );
    int h = metrics.height();

    QPainterPath path;
    QRect r( d->valuesRect );
    r.translate( -13, -13 );
    r.setWidth( r.width() + 26 );
    r.setHeight( r.height() + 26 );
    path.addEllipse( r );

    QPainterPath boundingPath;
    boundingPath.addEllipse( d->valuesRect );

    for ( int i = 0; i < rows; ++i )
    {
        QString s( this->model()->headerData( i, Qt::Vertical ).toString() );
        QRect textRect;

        qreal start = (rows - 1 - i) * this->stepSize() + this->startAngle();
        qreal ratio = ( start + angle/2.0 ) / 360.0 + 0.5;
        if ( ratio > 1 ) ratio -=1;
        QPointF p = path.pointAtPercent( ratio );
        QPointF p1 = boundingPath.pointAtPercent( ratio );

        /*painter.save();
        //painter.setPen( Qt::NoPen );
        painter.setPen( QPen( QColor( 150, 150, 150, 30 ), 3 ) );
        painter.drawPie( d->valuesRect, (start+delta/2) * 16, (angle-delta) * 16 );
        painter.restore();*/

        if ( p.y() <= p1.y() )   // To the top
        {
            textRect.setY( p.y() - h );
        }
        else                     // To the bottom
        {
            textRect.setY( p.y() );
        }

        textRect.setHeight( h );

        if ( p.x() >= p1.x() )   // To the right
        {
            textRect.setX( p.x() );
            textRect.setWidth( d->chartRect.right() - p.x() );
            painter.drawText( textRect, Qt::AlignLeft, s );

        }
        else                    // To the left
        {
            textRect.setX( d->chartRect.left() );
            textRect.setWidth( p.x() - d->chartRect.left() );
            painter.drawText( textRect, Qt::AlignRight, s );
        }
    }
}

qreal RadialAxis::startAngle() const
{
    return (90 + 10);
}


qreal RadialAxis::stepSize() const
{
    const Q_D( RadialAxis );
    return (360.0 - 20) / qreal( d->model->rowCount() );
}

}