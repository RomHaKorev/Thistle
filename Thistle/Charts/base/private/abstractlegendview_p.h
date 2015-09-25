#ifndef THISTLE_ABSTRACTLEGENDVIEW_P_H
#define THISTLE_ABSTRACTLEGENDVIEW_P_H

#include <QPainter>
#include <QRect>

#include "../../../kernel/private/abstractitemview_p.h"

#include <QVector>

namespace Thistle
{

class AbstractLegendView;
class AbstractChart;

class DotDelegate;
class BarDelegate;

struct AbstractLegendViewPrivate : public AbstractItemViewPrivate
{

	AbstractLegendView* q_ptr;
	AbstractChart* associatedChart;

	QVector<QRect> itemRects;
	QVector<QRect> test;
	QSize sizeHint;
	bool parentManageSize;

	AbstractLegendViewPrivate( AbstractLegendView* parent, AbstractChart* chart );
	virtual ~AbstractLegendViewPrivate();

	virtual void calculateSizeHint( const QSize& source )
	{
		Q_UNUSED( source )
	}
};

}

#endif // THISTLE_ABSTRACTLEGENDVIEW_P_H
