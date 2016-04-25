#include "pielegendview_p.h"

#include "../pielegendview.h"
#include "../../base/abstractlegendview.h"
#include "../../../Core/abstractitemview.h"
#include "../piechart.h"


namespace Thistle
{
PieLegendViewPrivate::PieLegendViewPrivate( PieLegendView* parent )
	: AbstractLegendViewPrivate( parent ), associatedPieChart( 0 )
{}


PieLegendViewPrivate::~PieLegendViewPrivate()
{}



} // namespace Thistle
