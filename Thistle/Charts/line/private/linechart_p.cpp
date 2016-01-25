#include "linechart_p.h"
#include "../linechart.h"
#include "../linechartlegend.h"
#include "../linechartcoordinateplaneview.h"
#include <qmath.h>

namespace Thistle
{

LineChartPrivate::LineChartPrivate( LineChart* q )
	: SerieChartPrivate( q, new LineChartCoordinatePlaneView() )
{}


LineChartPrivate::LineChartPrivate( LineChart* q, AbstractCoordinateSystemView* coordView )
	: SerieChartPrivate( q, coordView )
{}


LineChartPrivate::~LineChartPrivate()
{}

}
