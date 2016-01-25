#ifndef THISTLE_BASECHARTWIDGET_H
#define THISTLE_BASECHARTWIDGET_H

#include <QFrame>
#include <QVBoxLayout>
#include "private/basechartwidget_p.h"

#include "abstractlegendview.h"
#include "abstractchart.h"

class QAbstractItemModel;
class QLabel;

namespace Thistle
{

template<typename C, typename L> class BaseChartWidget : public QFrame
{
	Q_DECLARE_PRIVATE( BaseChartWidget );
protected:
	BaseChartWidgetPrivate* d_ptr;

	virtual void resizeEvent( QResizeEvent* ev )
	{
		BaseChartWidgetPrivate* const d = d_func();
		QWidget::resizeEvent( ev );
		d->resize( this->contentsRect() );
	}

public:
	Q_PROPERTY(QString title READ title WRITE setTitle )
	BaseChartWidget( QPointer<C> chart, QPointer<L> legend, QWidget* parent = 0 ):
	d_ptr( new BaseChartWidgetPrivate( parent, QPointer<AbstractChart>(chart), QPointer<AbstractLegendView>(legend) ) )
	{
		QPalette palette = this->palette();
		palette.setBrush( QPalette::Background, Qt::white );
		this->setPalette( palette );
		this->setAutoFillBackground( true );

		this->setContentsMargins( 20, 10, 20, 10 );
		QVBoxLayout* layout = new QVBoxLayout( this );
		layout->addWidget( d_ptr->legendView );
		layout->addWidget( d_ptr->chart );
		layout->addWidget( d_ptr->title, 1, Qt::AlignHCenter );
	}

	virtual ~BaseChartWidget()
	{}

	void setModel( QAbstractItemModel* model )
	{
		d_ptr->setModel( model );
	}

	QAbstractItemModel* model() const
	{
		return d_ptr->model();
	}


	void setTitle( const QString& title )
	{
		d_ptr->title->setText( title );
		d_ptr->resize( this->contentsRect() );
	}


	QString title() const
	{
		return d_ptr->title->text();
	}

	QFont font() const
	{
		return d_ptr->title->font();
	}

	void setFont( const QFont& font )
	{
		d_ptr->title->setFont( font );
		d_ptr->resize( this->contentsRect() );
	}

	QPointer<C> chart()
	{
		return QPointer<C>( qobject_cast<C*>(d_ptr->chart.data()) );
	}

	QPointer<L> legend()
	{
		return QPointer<L>( qobject_cast<L*>(d_ptr->legendView.data()) );
	}

};
}

#endif // THISTLE_BASECHARTWIDGET_H
