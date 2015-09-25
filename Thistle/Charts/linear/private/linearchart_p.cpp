#include "linearchart_p.h"
#include "../linearchart.h"
#include "../linearchartlegend.h"
#include "../linearchartcoordinateplaneview.h"
#include <qmath.h>

namespace Thistle
{

LinearChartPrivate::LinearChartPrivate( LinearChart* q )
	: SerieChartPrivate( q, new LinearChartCoordinatePlaneView() )
{}


LinearChartPrivate::LinearChartPrivate( LinearChart* q, AbstractCoordinateSystemView* coordView )
	: SerieChartPrivate( q, coordView )
{}


LinearChartPrivate::~LinearChartPrivate()
{}

}
