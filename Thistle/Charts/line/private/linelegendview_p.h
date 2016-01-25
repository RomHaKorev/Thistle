#ifndef THISTLE_LINELEGENDVIEW_P_H
#define THISTLE_LINELEGENDVIEW_P_H

#include <QPainter>
#include <QRect>

#include "../../base/private/abstractlegendview_p.h"

#include <QVector>

namespace Thistle
{

class LineLegendView;
class LineChart;
class DotDelegate;

struct LineLegendViewPrivate : public AbstractLegendViewPrivate
{
	QPointer<DotDelegate> dotDelegate;

	LineLegendViewPrivate( LineLegendView* parent );
	virtual ~LineLegendViewPrivate();
};

}

#endif // THISTLE_LINEARLEGENDVIEW_P_H
