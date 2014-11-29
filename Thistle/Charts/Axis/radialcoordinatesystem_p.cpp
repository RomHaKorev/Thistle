#include "radialcoordinatesystem_p.h"

namespace Thistle
{

RadialCoordinateSystemPrivate::RadialCoordinateSystemPrivate()
    : AbstractCoordinateSystemPrivate()
{
    centerHoleDiam = 0;
    origin = QPointF( 0, 0 );
    //this->yAxis().setTicksCount( 5 );
    //this->yAxis().setPrecision( 0 );
}

}