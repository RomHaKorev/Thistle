#include "abstractcoordinatesystem_p.h"
#include "../../../kernel/global.h"

namespace Thistle
{
AbstractCoordinateSystemPrivate::AbstractCoordinateSystemPrivate()
	: rect(),
	  font(),
	  axisPen( QColor( 50, 50, 50 ) ),
	  tickPen( QColor( 90, 90, 90 ) ),
	  textPen( QColor( 20, 20, 20 ) ),
	  model( 0 ),
	  valueOffset( 0, 0 )
{}

AbstractCoordinateSystemPrivate::~AbstractCoordinateSystemPrivate()
{}

}
