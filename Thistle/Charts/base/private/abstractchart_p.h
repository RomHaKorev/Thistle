#ifndef THISTLE_ABSTRACTCHART_P_H
#define THISTLE_ABSTRACTCHART_P_H

#include <QRect>
#include <QString>
#include <QFont>

#include "../../../Core/private/abstractitemview_p.h"
//#include "Layers/abstractLayer.h"
#include "../serieformat.h"
#include "../serieformatproxy.h"

namespace Thistle
{
class AbstractChart;

struct AbstractChartPrivate : AbstractItemViewPrivate
{

	inline AbstractChart* q_func()
	{
		return q_ptr;
	}
	AbstractChart* q_ptr;

	AbstractChartPrivate( AbstractChart* q );
	virtual ~AbstractChartPrivate();

	void pressEvent();
	void releaseEvent();

#if 0
	std::list<AbstractLayer*> layers;
	void addLayer( AbstractLayer* layer );
	void applyLayer( QPainter& painter );
#endif
};

}

#endif // THISTLE_ABSTRACTCHART_P_H
