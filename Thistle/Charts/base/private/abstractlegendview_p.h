#ifndef THISTLE_ABSTRACTLEGENDVIEW_P_H
#define THISTLE_ABSTRACTLEGENDVIEW_P_H

#include <QPainter>
#include <QRect>

#include "../../../Core/private/abstractitemview_p.h"
#include "../serieformatproxy.h"

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
	QPointer<Thistle::SerieFormatProxy> formatProxy; /* Should not be explicitly deleted in the destructor. Could be shared. */
	QSize sizeHint;

	AbstractLegendViewPrivate( AbstractLegendView* parent );
	virtual ~AbstractLegendViewPrivate();

	virtual void calculateSizeHint( const QSize& source );

	virtual QRect serieRect( unsigned int serieIndex ) const;
};

}

#endif // THISTLE_ABSTRACTLEGENDVIEW_P_H
