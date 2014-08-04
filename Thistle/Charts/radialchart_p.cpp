#include "radialchart_p.h"
#include "radialchart.h"

#include "legends/radialchartlegend.h"

namespace Thistle {

    RadialChartPrivate::RadialChartPrivate( RadialChart* q ) : AxisChartPrivate( q ) {
        delete this->legend;
        this->legend = new RadialChartLegend( q );
    }

    RadialChartPrivate::~RadialChartPrivate() {
        if ( this->radialAxis != 0 ) delete this->radialAxis;
    }

}