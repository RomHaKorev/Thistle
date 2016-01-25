#ifndef THISTLE_LINECHARTWIDGET_P_H
#define THISTLE_LINECHARTWIDGET_P_H

#include <QtCore/QRect>
#include <QLabel>

#include "../linechart.h"
#include "../linelegendview.h"
#include "../../base/private/basechartwidget_p.h"

namespace Thistle
{

class LineLegendView;
class LineChartWidget;

struct LineChartWidgetPrivate: public BaseChartWidgetPrivate
{
    LineChartWidget* parent;
    LineChartWidgetPrivate( LineChartWidget* parent );
    ~LineChartWidgetPrivate();

	virtual void resize( const QRect& rect );

	void setModel( QAbstractItemModel* model );
	QAbstractItemModel* model() const;
};

}

#endif // THISTLE_LINECHARTWIDGET_P_H
