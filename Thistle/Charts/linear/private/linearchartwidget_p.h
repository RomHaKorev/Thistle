#ifndef THISTLE_LINEARCHARTWIDGET_P_H
#define THISTLE_LINEARCHARTWIDGET_P_H

#include <QtCore/QRect>
#include <QLabel>

#include "../linearchart.h"
#include "../linearlegendview.h"


namespace Thistle
{

class LinearLegendView;
class LinearChartWidget;

struct LinearChartWidgetPrivate
{
	LinearChartWidget* parent;
	LinearChart chart;
	LinearLegendView legendView;
	QLabel title;

	LinearChartWidgetPrivate( LinearChartWidget* parent );
	~LinearChartWidgetPrivate();

	void resize( const QRect& rect );

	void setModel( QAbstractItemModel* model );
	QAbstractItemModel* model() const;
};

}

#endif // THISTLE_LINEARCHARTWIDGET_P_H
