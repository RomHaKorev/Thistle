#ifndef THISTLE_PIECHARTWIDGET_P_H
#define THISTLE_PIECHARTWIDGET_P_H

#include <QtCore/QRect>
#include <QLabel>

#include "../piechart.h"
#include "../pielegendview.h"


namespace Thistle
{

class PieChartWidget;

struct PieChartWidgetPrivate
{
    PieChartWidget* parent;
    PieChart chart;
    PieLegendView legendView;
	QLabel title;

    PieChartWidgetPrivate( PieChartWidget* parent );
    ~PieChartWidgetPrivate();

	void resize( const QRect& rect );

	void setModel( QAbstractItemModel* model );
	QAbstractItemModel* model() const;
};

}

#endif // THISTLE_PIECHARTWIDGET_P_H
