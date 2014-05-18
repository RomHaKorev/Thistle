#include "abstractchart_p.h"

#include "legends/chartlegend.h"

namespace Thistle {

    AbstractChartPrivate::AbstractChartPrivate( AbstractChart* q ) : q_ptr( q ), titleFont() {
        this->titleFont.setPixelSize( 14 );
        this->titleFont.setItalic( true );
        this->legend = new ChartLegend( q );
    }
}