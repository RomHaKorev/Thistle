#include "kiviatchartwidget.h"
#include "private/kiviatchartwidget_p.h"
#include "../line/linelegendview.h"
#include "kiviatchart.h"

#include <QResizeEvent>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QAbstractItemModel>
#include <QLabel>

namespace Thistle
{

KiviatChartWidget::KiviatChartWidget( QWidget *parent )
	: QFrame( parent ), d_ptr( new KiviatChartWidgetPrivate( this ) )
{
	this->setContentsMargins( 20, 10, 20, 10 );

	QPalette palette = this->palette();
	palette.setBrush( QPalette::Background, Qt::white );
	this->setPalette( palette );
}


KiviatChartWidget::~KiviatChartWidget()
{}


KiviatChart& KiviatChartWidget::chart()
{
	Q_D( KiviatChartWidget );
	return d->chart;
}


void KiviatChartWidget::resizeEvent( QResizeEvent* ev )
{
	Q_D( KiviatChartWidget );

	QWidget::resizeEvent( ev );

	d->resize( this->contentsRect() );
}


void KiviatChartWidget::setModel( QAbstractItemModel* model )
{
	Q_D( KiviatChartWidget );
	d->setModel( model );
}


QAbstractItemModel* KiviatChartWidget::model() const
{
	const Q_D( KiviatChartWidget );
	return d->model();
}


void KiviatChartWidget::setTitle( const QString& title )
{
	Q_D( KiviatChartWidget );
	d->title.setText( title );
	d->resize( this->contentsRect() );
}


QString KiviatChartWidget::title() const
{
	const Q_D( KiviatChartWidget );

	return d->title.text();

}


QFont KiviatChartWidget::font() const
{
	const Q_D( KiviatChartWidget );
	return d->title.font();
}


void KiviatChartWidget::setFont( const QFont& font )
{
	Q_D( KiviatChartWidget );
	d->title.setFont( font );
	d->resize( this->contentsRect() );
}

}
