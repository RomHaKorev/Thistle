#ifndef THISTLE_LINEARLEGENDVIEW_H
#define THISTLE_LINEARLEGENDVIEW_H

#include <QPainter>
#include <QRect>

#include "../base/abstractlegendview.h"

namespace Thistle
{

class ChartWidget;
class LinearChart;
class LinearLegendViewPrivate;

class LinearLegendView : public AbstractLegendView
{
	Q_DECLARE_PRIVATE( LinearLegendView );
private:
	virtual void paintSerie( QPainter &painter, int serie, const QRect& rect ) const;

	bool isActiveColumn( int column ) const;
public:
	LinearLegendView( LinearChart* chart, QWidget* parent = 0 );
	virtual ~LinearLegendView();

	void updateSizeHint( const QSize& source );
};

}

#endif // THISTLE_LINEARLEGENDVIEW_H
