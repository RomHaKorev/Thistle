#include "basechartwidget_p.h"
#include "../basechartwidget.h"
#include "../abstractchart.h"
#include "../abstractlegendview.h"

#include <QStandardItemModel>

int foo()
{
	return 1;
}
namespace Thistle
{

BaseChartWidgetPrivate::BaseChartWidgetPrivate( QWidget* parent )
	: title( new QLabel( "", parent ) )
{
	legendView->setParent( parent );
	QFont f = this->title->font();
	f.setPixelSize( 14 );
	f.setItalic( true );
	this->title->setFont( f );
	this->title->setAlignment( Qt::AlignCenter );

	QStandardItemModel* model = new QStandardItemModel();
	this->setModel( model );
}

BaseChartWidgetPrivate::BaseChartWidgetPrivate( QWidget* p, QPointer<Thistle::AbstractChart> aChart, QPointer<Thistle::AbstractLegendView> aLegend )
	: chart( aChart ), legendView( aLegend ), title( new QLabel( "", p ) )
{}

BaseChartWidgetPrivate::~BaseChartWidgetPrivate()
{
	legendView->deleteLater();
	chart->deleteLater();
	title->deleteLater();
}

void BaseChartWidgetPrivate::setModel( QAbstractItemModel* model )
{
	this->chart->setModel( model );
	this->legendView->setModel( model );
	this->legendView->setSelectionModel( this->chart->selectionModel() );
}

QAbstractItemModel* BaseChartWidgetPrivate::model() const
{
	return this->chart->model();
}

void BaseChartWidgetPrivate::resize( const QRect& rect )
{
	this->legendView->updateSizeHint( rect.size() );
	this->legendView->setFixedHeight( legendView->sizeHint().height() + 20 );
}
}
