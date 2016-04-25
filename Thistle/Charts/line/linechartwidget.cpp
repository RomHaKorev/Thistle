#include "linechartwidget.h"
#include "linelegendview.h"
#include "linechart.h"

#include <QResizeEvent>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QAbstractItemModel>
#include <QLabel>

namespace Thistle
{

LineChartWidget::LineChartWidget( QWidget* parent )
	: BaseChartWidget<LineChart, LineLegendView>( new Thistle::LineChart(), new Thistle::LineLegendView( this ) )
{
	QPointer<Thistle::LineChart> c = this->chart();
	QPointer<Thistle::LineLegendView> l = this->legend();
	l->setSerieFormatProxy( c->serieFormatProxy() );
//	this->legend()->setSerieFormatProxy( this->chart()->serieFormatProxy() );
}


LineChartWidget::~LineChartWidget()
{}
}
