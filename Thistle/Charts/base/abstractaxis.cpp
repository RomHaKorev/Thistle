#include "abstractaxis.h"
#include "private/abstractaxis_p.h"
#include "../../kernel/global.h"

#include <qmath.h>

namespace Thistle
{

AbstractAxis::AbstractAxis(): d_ptr( new AbstractAxisPrivate() )
{}

AbstractAxis::AbstractAxis( AbstractAxisPrivate* d ): d_ptr( d )
{}

AbstractAxis::~AbstractAxis()
{
  if ( d_ptr )
    delete d_ptr;
  d_ptr = 0;
}

void AbstractAxis::setBounds( qreal min, qreal max )
{
    d_ptr->min = min;
    d_ptr->max = max;
    d_ptr->calculateBounds();
    qreal orderMin = Thistle::calculateOrder( min );
    qreal orderMax = Thistle::calculateOrder( max );

    if ( qFloor( qAbs(min) / orderMin ) > 12 )
        orderMin *= 10.0;
    else if ( min == 0 )
        orderMin = -1;

    if ( qFloor( qAbs(max) / orderMax ) > 12 )
        orderMax *= 10.0;
    else if ( max == 0 )
        orderMax = -1;

    this->setTickIncrement( qMax( orderMin, orderMax ) );

    if ( this->tickIncrement() >= 10 )
    {
        d_ptr->precision = 0;
    }
    else if ( this->tickIncrement() == 1 )
    {
        d_ptr->precision = 2;
    }
    else
    {
        int nbZero = QString::number( this->tickIncrement() ).count( "0" );
        d_ptr->precision = nbZero + 2;
    }
}

qreal AbstractAxis::minimum() const
{
  qreal exp = qPow( 10, this->precision() );
  qreal floor = d_ptr->minBound * exp;
    return floor / exp;
}

qreal AbstractAxis::maximum() const
{
  qreal exp = qPow( 10, this->precision() );
  qreal floor = d_ptr->maxBound * exp;
  return floor / exp;
}

double AbstractAxis::order() const
{
    return d_ptr->order;
}

int AbstractAxis::precision() const
{
    return d_ptr->precision;
}

qreal AbstractAxis::tickIncrement() const
{
    return d_ptr->tickIncrement;
}

int AbstractAxis::ticksCount() const
{
    return d_ptr->ticksCount;
}

qreal AbstractAxis::labelsLength() const
{
    return d_ptr->labelsLength;
}

void AbstractAxis::setPrecision( int precision )
{
    d_ptr->precision = precision;
}

void AbstractAxis::setTicksCount( int count )
{
    d_ptr->ticksCount = count;
}

void AbstractAxis::setLabelsLength( int length )
{
    d_ptr->labelsLength = length;
}

void AbstractAxis::setTickIncrement( qreal size )
{
    d_ptr->tickIncrement = size;
}

void AbstractAxis::setOrder( double order )
{
    d_ptr->order = order;
}


void AbstractAxis::setName( const QString& name )
{
	d_ptr->name = name;
}

QString AbstractAxis::name() const
{
	return d_ptr->name;
}


}
