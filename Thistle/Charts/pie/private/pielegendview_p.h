#ifndef THISTLE_PIELEGENDVIEW_P_H
#define THISTLE_PIELEGENDVIEW_P_H

#include "../../base/private/abstractlegendview_p.h"

namespace Thistle
{

class PieLegendView;
class PieChart;

class PieLegendViewPrivate : public AbstractLegendViewPrivate
{
public:
	PieLegendViewPrivate( PieLegendView* parent, PieChart* associatedChart );
	~PieLegendViewPrivate();
	QPointer<PieChart> associatedPieChart;
};

} // namespace Thistle

#endif // THISTLE_PIELEGENDVIEW_P_H
