#include "radarchart_p.h"
#include "radarchart.h"

#include "legends/linearchartlegend.h"

namespace Thistle
{

RadarChartPrivate::RadarChartPrivate( RadarChart* q ) : LinearChartPrivate( q )
{
    this->legend = new LinearChartLegend( q );
}

RadarChartPrivate::~RadarChartPrivate()
{
    if ( this->radarAxis != 0 ) delete this->radarAxis;
}
}