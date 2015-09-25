#ifndef THISTLE_SCATTERCHARTWIDGET_P_H
#define THISTLE_SCATTERCHARTWIDGET_P_H

#include <QtCore/QRect>
#include <QLabel>

#include "../scatterchart.h"
#include "../../linear/linearlegendview.h"


namespace Thistle
{

class LinearLegendView;
class ScatterChartWidget;

struct ScatterChartWidgetPrivate
{
	ScatterChartWidget* parent;
	LinearChart chart;
	LinearLegendView legendView;
	QLabel title;

	ScatterChartWidgetPrivate( ScatterChartWidget* parent );
	~ScatterChartWidgetPrivate();

	void resize( const QRect& rect );

	void setModel( QAbstractItemModel* model );
	QAbstractItemModel* model() const;
};

}

#endif // THISTLE_SCATTERCHARTWIDGET_P_H
