#ifndef THISTLE_LINELEGENDVIEW_H
#define THISTLE_LINELEGENDVIEW_H

#include <QPainter>
#include <QRect>

#include "../base/abstractlegendview.h"

namespace Thistle
{

class ChartWidget;
class LineChart;
class LineLegendViewPrivate;

class LineLegendView : public AbstractLegendView
{
	Q_OBJECT
	Q_DECLARE_PRIVATE( LineLegendView )
private:
	virtual void paintSeriePicto( QPainter &painter, const QRect& rect, int serie ) const;

	bool isActiveColumn( int column ) const;
public:
	LineLegendView( QWidget* parent = 0 );
	virtual ~LineLegendView();

	virtual unsigned int serieCount() const;
	virtual QString serieName( unsigned int serieIdx ) const;
};

}

#endif // THISTLE_LINELEGENDVIEW_H
