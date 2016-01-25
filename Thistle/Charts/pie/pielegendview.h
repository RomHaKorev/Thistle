#ifndef THISTLE_PIELEGENDVIEW_H
#define THISTLE_PIELEGENDVIEW_H

#include "../base/abstractlegendview.h"

namespace Thistle
{
class PieChart;
class PieLegendViewPrivate;

class PieLegendView : public AbstractLegendView
{
	Q_DECLARE_PRIVATE( PieLegendView )
protected:
	virtual void paintSeriePicto( QPainter &painter, const QRect& rect, int serie ) const;
public:
	PieLegendView( PieChart* chart, QWidget* parent = 0 );
	virtual ~PieLegendView();
	virtual unsigned int serieCount() const;
	virtual QString serieName( unsigned int serieIdx ) const;
};
}

#endif // THISTLE_PIELEGENDVIEW_H
