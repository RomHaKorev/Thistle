#include "scatterchartwidget.h"
#include "private/scatterchartwidget_p.h"
#include "../linear/linearlegendview.h"
#include "scatterchart.h"

#include <QResizeEvent>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QAbstractItemModel>
#include <QLabel>

namespace Thistle
{

ScatterChartWidget::ScatterChartWidget(QWidget *parent)
	: QWidget( parent ), d_ptr( new ScatterChartWidgetPrivate( this ) )
{
	this->setContentsMargins( 20, 10, 20, 10 );

	QPalette palette = this->palette();
	palette.setBrush( QPalette::Background, Qt::white );
	this->setPalette( palette );
}


ScatterChartWidget::~ScatterChartWidget()
{}


LinearChart& ScatterChartWidget::chart()
{
	Q_D( ScatterChartWidget );
	return d->chart;
}


void ScatterChartWidget::resizeEvent( QResizeEvent* ev )
{
	Q_D( ScatterChartWidget );
	QWidget::resizeEvent( ev );
	d->resize( this->contentsRect() );
}


void ScatterChartWidget::setModel( QAbstractItemModel* model )
{
	Q_D( ScatterChartWidget );
	d->setModel( model );
}


QAbstractItemModel* ScatterChartWidget::model() const
{
	const Q_D( ScatterChartWidget );
	return d->model();
}


void ScatterChartWidget::setTitle( const QString& title )
{
	Q_D( ScatterChartWidget );
	d->title.setText( title );
}


QString ScatterChartWidget::title() const
{
	const Q_D( ScatterChartWidget );
	return d->title.text();

}


QFont ScatterChartWidget::font() const
{
	const Q_D( ScatterChartWidget );
	return d->title.font();
}


void ScatterChartWidget::setFont( const QFont& font )
{
	Q_D( ScatterChartWidget );
	d->title.setFont( font );
	d->resize( this->contentsRect() );
}

}
