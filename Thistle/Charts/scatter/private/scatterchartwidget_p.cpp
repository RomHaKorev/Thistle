#include "scatterchartwidget_p.h"
#include "../scatterchartwidget.h"


#include <QAbstractItemModel>

namespace Thistle
{

ScatterChartWidgetPrivate::ScatterChartWidgetPrivate( ScatterChartWidget* p )
	: parent( p ), chart( parent ), legendView( &chart ), title( "", p )
{
	legendView.setParent( parent );
	QFont f = this->title.font();
	f.setPixelSize( 14 );
	f.setItalic( true );
	this->title.setFont( f );
	this->title.setAlignment( Qt::AlignCenter );
}

ScatterChartWidgetPrivate::~ScatterChartWidgetPrivate()
{}

void ScatterChartWidgetPrivate::setModel( QAbstractItemModel* model )
{
	this->chart.setModel( model );
	this->legendView.setModel( model );
	this->legendView.setSelectionModel( this->chart.selectionModel() );
}

QAbstractItemModel* ScatterChartWidgetPrivate::model() const
{
	return this->chart.model();
}

void ScatterChartWidgetPrivate::resize( const QRect& rect )
{
	this->legendView.updateSizeHint( rect.size() );
	this->legendView.setMinimumHeight( legendView.sizeHint().height() );

	int marginBottom = parent->contentsMargins().bottom();

	QPoint p( rect.topLeft() );
	QRect r( p, this->legendView.sizeHint() );

	this->legendView.setGeometry( r );
	this->chart.setGeometry( QRect( r.bottomLeft() + QPoint( 0, marginBottom ),
									QSize( rect.width(), rect.height() - r.height() - marginBottom * 2 ) ) );

	int height = rect.height() - r.height() - marginBottom * 2;

	if ( this->title.text() != "" )
	{
		height -= this->title.height() + marginBottom;
	}

	this->chart.setGeometry( QRect( r.bottomLeft() + QPoint( 0, marginBottom ),
									QSize( rect.width(), height ) ) );

	if ( this->title.text() == "" )
		return;

	r = this->chart.geometry();

	this->title.setGeometry( QRect( r.bottomLeft() + QPoint( 0, marginBottom ),
									QSize( rect.width(), this->title.sizeHint().height() ) ) );
}

}
