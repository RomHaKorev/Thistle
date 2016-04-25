#include "piechart3dwidget.h"
#include "pielegendview.h"
#include "piechart3d.h"

#include <QResizeEvent>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QAbstractItemModel>
#include <QLabel>

namespace Thistle
{

PieChart3DWidget::PieChart3DWidget( QWidget* parent )
	: BaseChartWidget<PieChart3D, PieLegendView>( new Thistle::PieChart3D(), new Thistle::PieLegendView( this ) )
{
	//QPointer<Thistle::PieChart> c = qobject_cast<QPointer<Thistle::PieChart> >( this->chart() );
	QPointer<Thistle::PieChart> c = this->chart();
	QPointer<Thistle::PieLegendView> l = this->legend();
	l->setAssociatedPieChart( c );
}


PieChart3DWidget::~PieChart3DWidget()
{}
}
