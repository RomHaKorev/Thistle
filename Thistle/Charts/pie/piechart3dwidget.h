#ifndef THISTLE_PIECHART3DWIDGET_H
#define THISTLE_PIECHART3DWIDGET_H

#include "../base/basechartwidget.h"

class QAbstractItemModel;
class QLabel;

namespace Thistle
{

class PieChart3D;
class PieLegendView;

class PieChart3DWidget : public BaseChartWidget<PieChart3D, PieLegendView>
{
	Q_OBJECT
public:
	PieChart3DWidget( QWidget* parent = 0 );
	virtual ~PieChart3DWidget();
};
}

#endif // THISTLE_PIECHART3DWIDGET_H
