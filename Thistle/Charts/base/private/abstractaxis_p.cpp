#include "abstractaxis_p.h"
#include "../../../kernel/global.h"

namespace Thistle
{

AbstractAxisPrivate::AbstractAxisPrivate() : minBound(0), maxBound(0), min(0), max(0), order(1), precision(3), ticksCount(10), tickSize(1), labelsLength(20)
 {}

void AbstractAxisPrivate::calculateBounds()
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
    this->precision = 4;
}


}
