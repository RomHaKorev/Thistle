#include "cartesiancoordinatesystem.h"
#include "cartesiancoordinatesystem_p.h"
#include "../../kernel/global.h"

#include <QFontMetrics>
#include <QAbstractItemModel>
#include <QPainter>

namespace Thistle
{

CartesianCoordinateSystem::CartesianCoordinateSystem() : AbstractCoordinateSystem( new CartesianCoordinateSystemPrivate() )
{}

CartesianCoordinateSystem::CartesianCoordinateSystem( CartesianCoordinateSystemPrivate* d ) : AbstractCoordinateSystem( d )
{}

qreal CartesianCoordinateSystem::calculateOrder( qreal value )
{
    qreal order = 1.0;
    qreal v = qAbs( value );
    if ( v >= 1 )
    {
        while ( v > 1 )
        {
            order *= 10.0;
            v /= 10.0;
        }
        order /= 10.0;
    }
    else if ( v != 0 )
    {
        while ( v < 1 )
        {
            order /= 10.0;
            v *= 10.0;
        }
        order *= 10.0;
    }
    return order;
}


QFont CartesianCoordinateSystem::font() const
{
    return QFont();
}



QPointF CartesianCoordinateSystem::origin() const
{
    const Q_D( CartesianCoordinateSystem );
    return d->xAxis().origin();
}


void CartesianCoordinateSystem::paintBack( QPainter& painter ) const
{
    const Q_D( CartesianCoordinateSystem );
    painter.save();
    painter.setPen( d->axisPen );
    painter.setRenderHint( QPainter::Antialiasing, false );
    this->paintXAxis( painter );
    this->paintYAxis( painter );
    painter.restore();
}


void CartesianCoordinateSystem::paintFront( QPainter& painter ) const
{
    const Q_D( CartesianCoordinateSystem );
    painter.save();
    QFontMetrics metrics( d->font );
    int h = metrics.height();
    QPoint textPos( h/2 , this->origin().y() + 5 );
    int n = d->model->rowCount();
    for ( int i = 0; i < n; ++i )
    {
        QString s( d->model->headerData( i, Qt::Vertical ).toString() );
        s = metrics.elidedText( s, Qt::ElideRight, d->yAxis().labelsLength()  - 3, Qt::TextWrapAnywhere );
        QPointF x = d->xAxis().line().pointAt( float(i)/float(n) );
        QPoint p1( x.x(), this->origin().y() - 3 );
        painter.save();
        painter.setPen( d->textPen );
        if ( d->verticalLabels == true )
        {
            painter.rotate( -90 );
            if ( d->startOnAxis == false )
            {
                painter.translate( -textPos.y() - d->yAxis().labelsLength() - 3 , p1.x() + this->stepSize()/2.0 );
            }
            else
            {
                painter.translate( -textPos.y() - d->yAxis().labelsLength() - 3 , p1.x() + h );
            }
            QRect r( 0, 0, d->yAxis().labelsLength(), metrics.height() );
            painter.drawText( r, Qt::AlignRight, s );
        }
        else
        {
            if ( d->startOnAxis == false )
            {
                painter.drawText( p1.x() + this->stepSize() / 2.0 - d->yAxis().labelsLength() / 2.0, textPos.y() + h, s );
            }
            else
            {
                painter.drawText( p1.x(), textPos.y() + h, s );
            }
        }
        painter.restore();
    }
    painter.restore();
}


void CartesianCoordinateSystem::paintXAxis( QPainter& painter ) const
{
    const Q_D( CartesianCoordinateSystem );
    int n = d->model->rowCount();
    for ( int i = 0; i < n; ++ i )
    {
        QPointF x = d->xAxis().line().pointAt( float(i)/float(n) );
        QLineF l( x - QPoint(0, 3), x + QPoint(0, 3) );
        painter.drawLine( l );
    }
    painter.drawLine( d->xAxis().line() );
}


void CartesianCoordinateSystem::paintYAxis( QPainter& painter ) const
{
    /*Paints text on the X & Y axis.*/
    const Q_D( CartesianCoordinateSystem );
    painter.save();
    painter.setPen( d->tickPen );
    QFontMetrics metrics( d->font );
    int h = metrics.height();
    QPoint textPos( h/2 , this->origin().y() + 5 );
    qreal x = this->stepSize() + this->origin().x();
    int i = 0;
    qreal order = d->yAxis().tickSize();
    int y = order * 2;
    while ( y <= d->yAxis().maximum() )
    {
        QPoint p1( this->origin().x(), this->valueToPoint(y, 0).y() );
        QPoint p2 = p1 + QPoint( d->xAxis().line().length(), 0 );
        QLineF l( p1, p2 );
        QLineF::IntersectType intersectType;
        QPointF intersectionPoint;
        intersectType = l.intersect( d->xAxis().line(), &intersectionPoint );
        if ( intersectType == QLineF::BoundedIntersection )
        {
            l.setP2( intersectionPoint );
        }
        painter.setPen( d->tickPen );
        painter.drawLine( l );
        p1 = QPoint( this->origin().x(), this->valueToPoint( y, 0).y() );
        QString s = QString::number( y, 'f', d->yAxis().precision() );
        QRect r( QPoint( 0, p1.y() - h/2 ), QSize( this->origin().x() - 5 ,h) );
        painter.setPen( d->textPen );
        painter.drawText( r, Qt::AlignRight, s );
        y += order;
    }
    y = order;

    while ( y >= d->yAxis().minimum() )
    {
        QPoint p1( this->origin().x(), this->valueToPoint(y, 0).y() );
        if ( qAbs(p1.y() - this->origin().y() ) > 10 )
        {
            QPoint p2 = p1 + QPoint( d->xAxis().line().length(), 0 );
            QLineF l( p1, p2 );
            QLineF::IntersectType intersectType;
            QPointF intersectionPoint;
            intersectType = l.intersect( d->xAxis().line(), &intersectionPoint );
            if ( intersectType == QLineF::BoundedIntersection )
            {
                l.setP2( intersectionPoint );
            }
            painter.setPen( d->tickPen );
            painter.drawLine( l );
            p1 = QPoint( this->origin().x(), this->valueToPoint( y, 0).y() );
            QString s = QString::number( y, 'f', d->yAxis().precision() );
            QRect r( QPoint( 0, p1.y() - h/2 ), QSize( this->origin().x() - 5 ,h) );
            painter.setPen( d->textPen );
            painter.drawText( r, Qt::AlignRight, s );
        }
        y -= order;
    }
    painter.restore();
    painter.drawLine( d->yAxis().line() );
}


void CartesianCoordinateSystem::setstartOnAxis( bool startOn )
{
    Q_D( CartesianCoordinateSystem );
    d->startOnAxis = startOn;
}


bool CartesianCoordinateSystem::startOnAxis() const
{
    const Q_D( CartesianCoordinateSystem );
    return d->startOnAxis;
}


qreal CartesianCoordinateSystem::stepSize() const
{
    const Q_D( CartesianCoordinateSystem );
    return d->xAxis().line().length() / qreal( d->model->rowCount() );
}


void CartesianCoordinateSystem::update()
{
    Q_D( CartesianCoordinateSystem );
    if ( d->model == 0 )
    {
        return;
    }

    QLineF line( d->valuesRect.bottomLeft() - QPoint( 0, 10 ), d->valuesRect.topLeft() + QPoint( 0, 10 ) );
    this->setAxisLine( CartesianCoordinateSystem::Y, line );
    QPointF origin = this->valueToPoint( 0, 0 );
    
    line.setP1( QPoint( line.p1().x(), origin.y() ) );
    line.setP2( line.p1() + QPoint( d->valuesRect.width(), 0 ) );
  
    this->setAxisLine( CartesianCoordinateSystem::X, line );
    
    d->verticalLabels = ( this->stepSize() <= d->yAxis().labelsLength() );
    if ( d->verticalLabels == true )
    {
        qreal delta = qAbs( d->chartRect.bottom() - origin.y() );
        if ( delta < d->yAxis().labelsLength() )
        {
            d->valuesRect.setHeight( d->valuesRect.height() - ( d->yAxis().labelsLength() - delta ) );
            this->update();
        }
    }
}


QPointF CartesianCoordinateSystem::valueToPoint( qreal value, int axisNumber ) const
{
    const Q_D( CartesianCoordinateSystem );
    qreal y = d->yAxis().pinpoint( value ).y();
    qreal x = d->xAxis().pinpoint( axisNumber + 0.5 ).x();

    return QPointF( x, y );
}




qreal CartesianCoordinateSystem::labelsLength( CartesianCoordinateSystem::Axis axis ) const
{
    const Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        return d->xAxis().labelsLength();
    break;
    default:
        return d->yAxis().labelsLength();
    }

};

qreal CartesianCoordinateSystem::tickSize( CartesianCoordinateSystem::Axis axis ) const
{
    const Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        return d->xAxis().tickSize();
    break;
    default:
        return d->yAxis().tickSize();
    }

};

qreal CartesianCoordinateSystem::minimum( CartesianCoordinateSystem::Axis axis ) const
{
    const Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        return d->xAxis().minimum();
    break;
    default:
        return d->yAxis().minimum();
    }

};


qreal CartesianCoordinateSystem::maximum( CartesianCoordinateSystem::Axis axis ) const
{
    const Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        return d->xAxis().maximum();
    break;
    default:
        return d->yAxis().maximum();
    }

};

long CartesianCoordinateSystem::order( CartesianCoordinateSystem::Axis axis ) const
{
    const Q_D( CartesianCoordinateSystem );

    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        return d->xAxis().order();
    break;
    default:
        return d->yAxis().order();
    }

};


int CartesianCoordinateSystem::precision( CartesianCoordinateSystem::Axis axis ) const
{
    const Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        return d->xAxis().precision();
    break;
    default:
        return d->yAxis().precision();
    }

};


int CartesianCoordinateSystem::ticksCount( CartesianCoordinateSystem::Axis axis ) const
{
    const Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        return d->xAxis().ticksCount();
    break;
    default:
        return d->yAxis().ticksCount();
    }

};



void CartesianCoordinateSystem::setTickSize( CartesianCoordinateSystem::Axis axis, qreal size )
{
    Q_D( CartesianCoordinateSystem );
    if( size <= 0 )
        return;

    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        d->xAxis().setTickSize( size );
    break;
    default:
        d->yAxis().setTickSize(  size );
    }
};

void CartesianCoordinateSystem::setLabelsLength( CartesianCoordinateSystem::Axis axis, int length )
{
    Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        d->xAxis().setLabelsLength( length );
    break;
    default:
        d->yAxis().setLabelsLength( length );
    }
};

void CartesianCoordinateSystem::setBounds( CartesianCoordinateSystem::Axis axis, qreal minimum, qreal maximum )
{
    Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        d->xAxis().setBounds( minimum, maximum );
    break;
    default:
        d->yAxis().setBounds( minimum, maximum );
    }
};


void CartesianCoordinateSystem::setOrder( CartesianCoordinateSystem::Axis axis, long order )
{
    Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        d->xAxis().setOrder( order );
    break;
    default:
        d->yAxis().setOrder( order );
    }
};


void CartesianCoordinateSystem::setPrecision( CartesianCoordinateSystem::Axis axis, int precision )
{
    Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        d->xAxis().setPrecision( precision );
    break;
    default:
        d->yAxis().setPrecision( precision );
    }
};


void CartesianCoordinateSystem::setTicksCount( CartesianCoordinateSystem::Axis axis, int nb )
{
    Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        d->xAxis().setTicksCount( nb );
    break;
    default:
        d->yAxis().setTicksCount( nb );
    }
};


void CartesianCoordinateSystem::setAxisLine( CartesianCoordinateSystem::Axis axis, const QLineF& line )
{
     Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        d->xAxis().setLine( line );
    break;
    default:
        d->yAxis().setLine( line );
    }
}


const QLineF CartesianCoordinateSystem::axisLine(CartesianCoordinateSystem::Axis axis )
{
    Q_D( CartesianCoordinateSystem );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        return d->xAxis().line();
    break;
    default:
        return d->yAxis().line();
    }
}

}