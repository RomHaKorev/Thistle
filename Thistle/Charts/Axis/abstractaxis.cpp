#include "abstractaxis.h"
#include "../../kernel/global.h"
#include <QAbstractItemModel>

namespace Thistle
{

AbstractAxis::AbstractAxis() : d_ptr( new AbstractAxisPrivate() )
{
    d_ptr->tickSize = 0;
    d_ptr->nbTicks = 10;
    d_ptr->axisPen = QPen( QBrush(Thistle::Colors::Gray), 1.5 );
    d_ptr->tickPen = QPen( QBrush(Thistle::Colors::LightGray), 0 );
    d_ptr->textPen = QPen( QBrush(Thistle::Colors::DarkGray), 1 );
}

AbstractAxis::AbstractAxis( AbstractAxisPrivate* d ) : d_ptr( d )
{
}


void AbstractAxis::calculateBounds()
{
    d_ptr->minBound = d_ptr->min;
    d_ptr->maxBound = d_ptr->max;
    if ( d_ptr->maxBound == d_ptr->minBound )
    {
        ++d_ptr->maxBound;
        --d_ptr->minBound;
    }
    d_ptr->order = calculateOrder( d_ptr->max - d_ptr->min );
    d_ptr->tickSize = (d_ptr->max - d_ptr->min ) / (d_ptr->nbTicks - 1);
    if ( d_ptr->order >= 10 )
    {
        d_ptr->nbDigits = 0;
    }
    else if ( d_ptr->order == 1 )
    {
        d_ptr->nbDigits = 2;
    }
    else
    {
        int nbZero = QString::number(d_ptr->order).count( "0" );
        d_ptr->nbDigits = nbZero + 2;
    }
}

long AbstractAxis::calculateOrder( qreal value ) const
{
    /*Calculates the d_ptr->order of scale for value (i.e. the power of ten greater than value).*/
    long order = 1.0;
    qreal v = abs( value );
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


}
