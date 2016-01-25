#include "pielegendview_p.h"

#include "../pielegendview.h"
#include "../../base/abstractlegendview.h"
#include "../../../Core/abstractitemview.h"
#include "../piechart.h"


namespace Thistle
{
PieLegendViewPrivate::PieLegendViewPrivate( PieLegendView* parent, PieChart* associatedChart )
	: AbstractLegendViewPrivate( parent ), associatedPieChart( associatedChart )
{}


PieLegendViewPrivate::~PieLegendViewPrivate()
{}



} // namespace Thistle
