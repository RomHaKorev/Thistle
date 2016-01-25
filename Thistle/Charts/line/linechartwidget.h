#ifndef THISTLE_LINECHARTWIDGET_H
#define THISTLE_LINECHARTWIDGET_H

#include "../base/basechartwidget.h"

class QAbstractItemModel;
class QLabel;

namespace Thistle
{

class LineChart;
class LineLegendView;

class LineChartWidget : public BaseChartWidget<LineChart, LineLegendView>
{
	Q_OBJECT
public:
	LineChartWidget( QWidget* parent = 0 );
	virtual ~LineChartWidget();
};
}

#endif // THISTLE_LINECHARTWIDGET_H
