#include "axisview.h"
#include "axisview_p.h"

#include "../../kernel/global.h"

#include <QFontMetrics>
#include <QAbstractItemModel>
#include <QPainter>

namespace Thistle
{

AxisView::AxisView() : d_ptr( new AxisViewPrivate() )
{}

void AxisView::setModel( QAbstractItemModel* model )
{
    d_ptr->model = model;
    this->update();
}


QAbstractItemModel* AxisView::model() const
{
    return d_ptr->model;
}


QFont AxisView::font() const
{
    return d_ptr->font;
}

void AxisView::setFont( const QFont& font )
{
    d_ptr->font = font;
    this->update();
}


qreal AxisView::stepSize() const
{
    return 0;
}


void AxisView::paintBack( QPainter& painter ) const
{
    painter.save();
    painter.setPen( d_ptr->axisPen );
    painter.setRenderHint( QPainter::Antialiasing, false );
    this->paintXAxis( painter );
    this->paintYAxis( painter );
    painter.restore();
}


void AxisView::paintFront( QPainter& painter ) const
{
    painter.save();
    QFontMetrics metrics( d_ptr->font );
    int h = metrics.height();
    QPoint textPos( h/2 , d_ptr->coordinateSystem->origin().y() + 5 );
    int n = d_ptr->model->rowCount();
    for ( int i = 0; i < n; ++i )
    {
        QString s( d_ptr->model->headerData( i, Qt::Vertical ).toString() );
        s = metrics.elidedText( s, Qt::ElideRight, d_ptr->coordinateSystem->labelsLength( CartesianCoordinateSystem::Y ) - 3, Qt::TextWrapAnywhere );
        QPointF x = d_ptr->coordinateSystem->axisLine( CartesianCoordinateSystem::X ).pointAt( float(i)/float(n) );
        QPoint p1( x.x(), d_ptr->coordinateSystem->origin().y() - 3 );
        painter.save();
        painter.setPen( d_ptr->textPen );
        if ( /*d_ptr->verticalLabels == true*/ false )
        {
            painter.rotate( -90 );
            if ( true /*d->startOnAxis == false*/ )
            {
                painter.translate( -textPos.y() - d_ptr->coordinateSystem->labelsLength( CartesianCoordinateSystem::Y ) - 3 , p1.x() + this->stepSize()/2.0 );
            }
            else
            {
                painter.translate( -textPos.y() - d_ptr->coordinateSystem->labelsLength( CartesianCoordinateSystem::Y ) - 3 , p1.x() + h );
            }
            QRect r( 0, 0, d_ptr->coordinateSystem->labelsLength( CartesianCoordinateSystem::Y ), metrics.height() );
            painter.drawText( r, Qt::AlignRight, s );
        }
        else
        {
            if ( /*d_ptr->coordinateSystem->startOnAxis == false*/ true )
            {
                painter.drawText( p1.x() + this->stepSize() / 2.0 - d_ptr->coordinateSystem->labelsLength( CartesianCoordinateSystem::Y ) / 2.0, textPos.y() + h, s );
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


void AxisView::paintXAxis( QPainter& painter ) const
{
    int n = d_ptr->model->rowCount();
    for ( int i = 0; i < n; ++ i )
    {
        QPointF x = d_ptr->coordinateSystem->axisLine( CartesianCoordinateSystem::X ).pointAt( float(i)/float(n) );
        QLineF l( x - QPoint(0, 3), x + QPoint(0, 3) );
        painter.drawLine( l );
    }
    painter.drawLine( d_ptr->coordinateSystem->axisLine( CartesianCoordinateSystem::X ) );
}


void AxisView::paintYAxis( QPainter& painter ) const
{
    /*Paints text on the X & Y axis.*/
    painter.save();
    painter.setPen( d_ptr->tickPen );
    QFontMetrics metrics( d_ptr->font );
    int h = metrics.height();
    QPoint textPos( h/2 , d_ptr->coordinateSystem->origin().y() + 5 );
    qreal x = this->stepSize() + d_ptr->coordinateSystem->origin().x();
    int i = 0;
    qreal order = d_ptr->coordinateSystem->tickSize( CartesianCoordinateSystem::Y );
    int y = order * 2;

    while ( y <= d_ptr->coordinateSystem->maximum( CartesianCoordinateSystem::Y ) )
    {
        QPoint p1( d_ptr->coordinateSystem->origin().x(), d_ptr->coordinateSystem->valueToPoint(y, 0).y() );
        QPoint p2 = p1 + QPoint( d_ptr->coordinateSystem->axisLine( CartesianCoordinateSystem::X ).length(), 0 );
        QLineF l( p1, p2 );
        QLineF::IntersectType intersectType;
        QPointF intersectionPoint;
        intersectType = l.intersect( d_ptr->coordinateSystem->axisLine( CartesianCoordinateSystem::X ), &intersectionPoint );
        if ( intersectType == QLineF::BoundedIntersection )
        {
            l.setP2( intersectionPoint );
        }
        painter.setPen( d_ptr->coordinateSystem->tickPen() );
        painter.drawLine( l );
        p1 = QPoint( d_ptr->coordinateSystem->origin().x(), d_ptr->coordinateSystem->valueToPoint( y, 0).y() );
        QString s = QString::number( y, 'f', d_ptr->coordinateSystem->precision( CartesianCoordinateSystem::Y ) );
        QRect r( QPoint( 0, p1.y() - h/2 ), QSize( d_ptr->coordinateSystem->origin().x() - 5 ,h) );
        painter.setPen( d_ptr->coordinateSystem->textPen() );
        painter.drawText( r, Qt::AlignRight, s );
        y += order;
    }
    y = order;

    while ( y >= d_ptr->coordinateSystem->minimum( CartesianCoordinateSystem::Y ) )
    {
        QPoint p1( this->origin().x(), d_ptr->coordinateSystem->valueToPoint(y, 0).y() );
        if ( qAbs(p1.y() - this->origin().y() ) > 10 )
        {
            QPoint p2 = p1 + QPoint( d_ptr->coordinateSystem->axisLine( CartesianCoordinateSystem::X ).length(), 0 );
            QLineF l( p1, p2 );
            QLineF::IntersectType intersectType;
            QPointF intersectionPoint;
            intersectType = l.intersect( d_ptr->coordinateSystem->axisLine( CartesianCoordinateSystem::X ), &intersectionPoint );
            if ( intersectType == QLineF::BoundedIntersection )
            {
                l.setP2( intersectionPoint );
            }
            painter.setPen( d_ptr->coordinateSystem->tickPen() );
            painter.drawLine( l );
            p1 = QPoint( this->origin().x(), d_ptr->coordinateSystem->valueToPoint( y, 0).y() );
            QString s = QString::number( y, 'f', d_ptr->coordinateSystem->precision( CartesianCoordinateSystem::Y ) );
            QRect r( QPoint( 0, p1.y() - h/2 ), QSize( this->origin().x() - 5 ,h) );
            painter.setPen( d_ptr->coordinateSystem->tickPen() );
            painter.drawText( r, Qt::AlignRight, s );
        }
        y -= order;
    }
    painter.restore();
    painter.drawLine( d_ptr->coordinateSystem->axisLine( CartesianCoordinateSystem::Y ) );
}

#if 0
qreal AxisView::stepSize() const
{
    return d_ptr->coordinateSystem->axisLine( CartesianCoordinateSystem::Y ).length() / qreal( d->model->rowCount() );
        //xAxis().line().length() / qreal( d->model->rowCount() );
}


void AxisView::update()
{}


qreal AxisView::labelsLength( CartesianCoordinateSystem::Axis axis ) const
{
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        return d_ptr->xAxis().labelsLength();
    break;
    default:
        return d_ptr->yAxis().labelsLength();
    }

};

qreal AxisView::tickSize( CartesianCoordinateSystem::Axis axis ) const
{
    const Q_D( AxisView );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        return d_ptr->xAxis().tickSize();
    break;
    default:
        return d_ptr->yAxis().tickSize();
    }

};

int AxisView::ticksCount( CartesianCoordinateSystem::Axis axis ) const
{
    const Q_D( AxisView );
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        return d_ptr->xAxis().ticksCount();
    break;
    default:
        return d_ptr->yAxis().ticksCount();
    }

};



void AxisView::setTickSize( CartesianCoordinateSystem::Axis axis, qreal size )
{
    if( size <= 0 )
        return;

    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        d_ptr->xAxis().setTickSize( size );
    break;
    default:
        d_ptr->yAxis().setTickSize(  size );
    }
};

void AxisView::setLabelsLength( CartesianCoordinateSystem::Axis axis, int length )
{
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        d_ptr->xAxis().setLabelsLength( length );
    break;
    default:
        d_ptr->yAxis().setLabelsLength( length );
    }
};


void AxisView::setPrecision( CartesianCoordinateSystem::Axis axis, int precision )
{
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        d_ptr->xAxis().setPrecision( precision );
    break;
    default:
        d_ptr->yAxis().setPrecision( precision );
    }
};


void AxisView::setTicksCount( CartesianCoordinateSystem::Axis axis, int nb )
{
    switch( axis )
    {
    case CartesianCoordinateSystem::X:
        d_ptr->xAxis().setTicksCount( nb );
    break;
    default:
        d_ptr->yAxis().setTicksCount( nb );
    }
};
#endif

void AxisView::update()
{
    if ( d_ptr->model == 0 )
    {
        return;
    }

    QLineF line( d_ptr->rect.bottomLeft() - QPoint( 0, 10 ), d_ptr->rect.topLeft() + QPoint( 0, 10 ) );
    d_ptr->coordinateSystem->setAxisLine( CartesianCoordinateSystem::Y, line );
    QPointF origin = d_ptr->coordinateSystem->valueToPoint( 0, 0 );
    
    line.setP1( QPoint( line.p1().x(), origin.y() ) );
    line.setP2( line.p1() + QPoint( d_ptr->rect.width(), 0 ) );
    
    d_ptr->coordinateSystem->setAxisLine( CartesianCoordinateSystem::X, line );
    
    /*d->verticalLabels = ( this->stepSize() <= d_ptr->coordinateSystem->labelsLength( CartesianCoordinateSystem::Y) );
    if ( d->verticalLabels == true )
    {
        qreal delta = qAbs( d->chartRect.bottom() - origin.y() );
        if ( delta < d_ptr->coordinateSystem->labelsLength( CartesianCoordinateSystem::Y) )
        {
            d->valuesRect.setHeight( d->valuesRect.height() - ( d_ptr->coordinateSystem->labelsLength( CartesianCoordinateSystem::Y) - delta ) );
            this->update();
        }
    }*/
    int rows = this->model()->rowCount();
    int cols = this->model()->columnCount();
    QFontMetrics metrics( this->font() );
    int textWidth = 0;
    int valueWidth = 0;
    qreal value = this->model()->index( 0, 0 ).data().toDouble();
    qreal min = 0;
    qreal max = 0;
    for ( int r = 0; r < rows; ++r )
    {
        QString s( this->model()->headerData( r, Qt::Vertical ).toString() );
        textWidth = qMax( textWidth, metrics.width( s ) + 5 );
        for ( int c = 0; c < cols; ++c )
        {
            value = this->model()->index( r, c ).data().toDouble();
            min = float( qMin( min, value ));
            max = float( qMax( max, value ));
            QString s = QString::number( value, 'f', d_ptr->coordinateSystem->precision( CartesianCoordinateSystem::Y ) );
            valueWidth = qMax( valueWidth, metrics.width( s ) );
        }
    }
    
    /*d->axis->setMin( min );
    d->axis->setMax( max );
    d->axis->setTickSize( qMax( d->axis->calculateOrder( min ), d->axis->calculateOrder( max) ) );
    d->axis->setYLabelsLength( valueWidth + 5 );
    d->axis->setXLabelsLength( textWidth );*/
    d_ptr->coordinateSystem->setBounds( CartesianCoordinateSystem::Y, min, max );
    d_ptr->coordinateSystem->setBounds( CartesianCoordinateSystem::X, 0, rows );
}


QPointF AxisView::origin() const
{
    return d_ptr->coordinateSystem->origin();
}


void AxisView::setRect( const QRect& rect )
{
    d_ptr->rect = rect;
}

CartesianCoordinateSystem* AxisView::coordinateSystem() const
{
    return d_ptr->coordinateSystem;
}

}