#include "abstractlegendview_p.h"
#include "../abstractlegendview.h"
#include "../../../kernel/abstractitemview.h"
#include "../abstractchart.h"

namespace Thistle
{
AbstractLegendViewPrivate::AbstractLegendViewPrivate( AbstractLegendView* parent, AbstractChart* chart )
	: AbstractItemViewPrivate( parent ), q_ptr( parent ), associatedChart( chart ), parentManageSize( false )
{}


AbstractLegendViewPrivate::~AbstractLegendViewPrivate()
{}

}
