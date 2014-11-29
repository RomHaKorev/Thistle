#include "chartview_p.h"
#include "chartview.h"
#include "legends/chartlegend.h"
#include "legends/linearchartlegend.h"
#include "axisview.h"

namespace Thistle
{

ChartViewPrivate::ChartViewPrivate( ChartView* q ) : q_ptr( q ), titleFont()
{
    this->titleFont.setPixelSize( 14 );
    this->titleFont.setItalic( true );
    this->legend = new ChartLegend( q );
    this->axisView = new AxisView();
}

ChartViewPrivate::~ChartViewPrivate()
{
    if ( this->legend != 0 ) delete this->legend;
}

}