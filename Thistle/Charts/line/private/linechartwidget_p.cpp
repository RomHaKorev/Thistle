#include "linechartwidget_p.h"
#include "../linechartwidget.h"

#include <QStandardItemModel>

namespace Thistle
{

LineChartWidgetPrivate::LineChartWidgetPrivate( LineChartWidget* p )
	: BaseChartWidgetPrivate( parent, new LineChart(parent), new LineLegendView( chart ) )
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

LineChartWidgetPrivate::~LineChartWidgetPrivate()
{
	legendView->deleteLater();
	chart->deleteLater();
	title->deleteLater();
}

void LineChartWidgetPrivate::setModel( QAbstractItemModel* model )
{
	this->chart->setModel( model );
	this->legendView->setModel( model );
	this->legendView->setSelectionModel( this->chart->selectionModel() );
}

QAbstractItemModel* LineChartWidgetPrivate::model() const
{
	return this->chart->model();
}

void LineChartWidgetPrivate::resize( const QRect& rect )
{
	this->legendView->updateSizeHint( rect.size() );
	this->legendView->setFixedHeight( legendView->sizeHint().height() + 20 );

	/*int marginBottom = parent->contentsMargins().bottom();

	QPoint p( rect.topLeft() );
	QRect r( p, this->legendView->sizeHint() );

	this->legendView->setGeometry( r );
	this->chart->setGeometry( QRect( r.bottomLeft() + QPoint( 0, marginBottom ),
									QSize( rect.width(), rect.height() - r.height() - marginBottom * 2 ) ) );

	int height = rect.height() - r.height() - marginBottom * 2;

	if ( this->title->text() != "" )
	{
		height -= this->title->height() + marginBottom;
	}

	this->chart->setGeometry( QRect( r.bottomLeft() + QPoint( 0, marginBottom ),
									QSize( rect.width(), height ) ) );

	if ( this->title->text() == "" )
		return;

	r = this->chart->geometry();

	this->title->setGeometry( QRect( r.bottomLeft() + QPoint( 0, marginBottom ),
									QSize( rect.width(), this->title->sizeHint().height() ) ) ); */
}

}
