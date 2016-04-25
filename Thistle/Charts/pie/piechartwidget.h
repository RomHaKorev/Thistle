#ifndef THISTLE_PIECHARTWIDGET_H
#define THISTLE_PIECHARTWIDGET_H

#include "../base/basechartwidget.h"

class QAbstractItemModel;
class QLabel;

namespace Thistle
{

class PieChart;
class PieLegendView;

class PieChartWidget : public BaseChartWidget<PieChart, PieLegendView>
{
	Q_OBJECT
public:
	PieChartWidget( QWidget* parent = 0 );
	virtual ~PieChartWidget();
};
}

#endif // THISTLE_PIECHARTWIDGET_H
