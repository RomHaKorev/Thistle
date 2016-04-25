#include "piechartwidget.h"
#include "pielegendview.h"
#include "piechart.h"

#include <QResizeEvent>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QAbstractItemModel>
#include <QLabel>

namespace Thistle
{

PieChartWidget::PieChartWidget( QWidget* parent )
	: BaseChartWidget<PieChart, PieLegendView>( new Thistle::PieChart(), new Thistle::PieLegendView( this ) )
{
	QPointer<Thistle::PieChart> c = this->chart();
	QPointer<Thistle::PieLegendView> l = this->legend();
	l->setAssociatedPieChart( c );
//	this->legend()->setSerieFormatProxy( this->chart()->serieFormatProxy() );
}


PieChartWidget::~PieChartWidget()
{}
}
