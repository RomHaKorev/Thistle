#include "linearchartwidget.h"
#include "private/linearchartwidget_p.h"
#include "linearlegendview.h"
#include "linearchart.h"

#include <QResizeEvent>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QAbstractItemModel>
#include <QLabel>

namespace Thistle
{

LinearChartWidget::LinearChartWidget(QWidget *parent)
    : QWidget( parent ), d_ptr( new LinearChartWidgetPrivate( this ) )
{
    this->setContentsMargins( 20, 10, 20, 10 );

    QPalette palette = this->palette();
    palette.setBrush( QPalette::Background, Qt::white );
    this->setPalette( palette );
}


LinearChartWidget::~LinearChartWidget()
{}


LinearChart* LinearChartWidget::chart() const
{
    const Q_D( LinearChartWidget );
    return d->chart;
}


void LinearChartWidget::resizeEvent( QResizeEvent* ev )
{
    Q_D( LinearChartWidget );

    QWidget::resizeEvent( ev );

    d->resize( this->contentsRect() );
}


void LinearChartWidget::setModel( QAbstractItemModel* model )
{
    Q_D( LinearChartWidget );
    d->setModel( model );
}


QAbstractItemModel* LinearChartWidget::model() const
{
    const Q_D( LinearChartWidget );
    return d->model();
}


void LinearChartWidget::setTitle( const QString& title )
{
    Q_D( LinearChartWidget );
    if ( d->title != 0 )
    {
        d->title->setText( title );
    }
}


QString LinearChartWidget::title() const
{
    const Q_D( LinearChartWidget );
    if ( d->title != 0 )
    {
        return d->title->text();
    }

    return "";
}


QFont LinearChartWidget::font() const
{
    const Q_D( LinearChartWidget );
    return d->title->font();
}


void LinearChartWidget::setFont( const QFont& font )
{
    Q_D( LinearChartWidget );
    d->title->setFont( font );
    d->resize( this->contentsRect() );
}

}
