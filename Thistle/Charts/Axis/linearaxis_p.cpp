#include "linearaxis_p.h"
#include "../../kernel/global.h"

namespace Thistle
{

LinearAxisPrivate::LinearAxisPrivate() : minBound(0), maxBound(0), min(0), max(0), order(1), precision(3), ticksCount(10), tickSize(1), labelsLength(20)
 {}

void LinearAxisPrivate::calculateBounds()
{
    this->minBound = this->min;
    this->maxBound = this->max;
    if ( this->maxBound == this->minBound )
    {
        ++this->maxBound;
        --this->minBound;
    }
    this->order = Thistle::calculateOrder( this->max - this->min );
    this->tickSize = (this->max - this->min ) / ( this->ticksCount - 1 );
    if ( this->order >= 10 )
    {
        this->precision = 0;
    }
    else if ( this->order == 1 )
    {
        this->precision = 2;
    }
    else
    {
        int nbZero = QString::number( this->order ).count( "0" );
        this->precision = nbZero + 2;
    }
}


}