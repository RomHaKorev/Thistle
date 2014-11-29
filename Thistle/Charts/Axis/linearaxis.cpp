#include "linearaxis.h"
#include "linearaxis_p.h"
#include "../../kernel/global.h"

#include <qmath.h>

namespace Thistle
{

LinearAxis::LinearAxis(): d_ptr( new LinearAxisPrivate() )
{}

void LinearAxis::setBounds( qreal min, qreal max )
{
    d_ptr->min = min;
    d_ptr->max = max;
    d_ptr->calculateBounds();
    this->setTickSize( qMax( Thistle::calculateOrder( min ), Thistle::calculateOrder( max) ) );
}

qreal LinearAxis::minimum() const
{
    return d_ptr->minBound;
}

qreal LinearAxis::maximum() const
{
    return d_ptr->maxBound;
}

long LinearAxis::order() const
{
    return d_ptr->order;
}

int LinearAxis::precision() const
{
    return d_ptr->precision;
}

qreal LinearAxis::tickSize() const
{
    return d_ptr->tickSize;
}

int LinearAxis::ticksCount() const
{
    return d_ptr->ticksCount;
}

qreal LinearAxis::labelsLength() const
{
    return d_ptr->labelsLength;
}

void LinearAxis::setPrecision( int precision )
{
    d_ptr->precision = precision;
}

void LinearAxis::setTicksCount( int count )
{
    d_ptr->ticksCount = count;
}

void LinearAxis::setLabelsLength( int length )
{
    d_ptr->labelsLength = length;
}

void LinearAxis::setTickSize( qreal size )
{
    d_ptr->tickSize = size;
}

void LinearAxis::setOrder( long order )
{
    d_ptr->order = order;
}

void LinearAxis::setLine( const QLineF& line )
{
    d_ptr->line = line;
}

QPointF LinearAxis::origin() const
{
    return d_ptr->line.p1();
}

const QLineF& LinearAxis::line() const
{
    return d_ptr->line;
}

QPointF LinearAxis::pinpoint( qreal value ) const
{
    qreal e = qreal( this->maximum() - this->minimum() );
    qreal ratio = ( value - this->minimum() ) / e;
    return d_ptr->line.pointAt( ratio );
}

}