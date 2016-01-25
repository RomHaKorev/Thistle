#include "pielegendview.h"
#include "piechart.h"
#include "private/pielegendview_p.h"

#include <QDebug>

namespace Thistle
{
PieLegendView::PieLegendView( PieChart* chart, QWidget* parent ) : AbstractLegendView( new PieLegendViewPrivate( this, chart ), parent )
{}


PieLegendView::~PieLegendView()
{}


void PieLegendView::paintSeriePicto( QPainter &painter, const QRect& rect, int serie ) const
{
	const Q_D( PieLegendView );

	painter.save();
	QColor color( this->model()->data( this->model()->index( serie, 0 ), Qt::DecorationRole ).toString() );
	if ( !color.isValid() )
		color = Colors::predefinedColor( serie );

	d->associatedPieChart->configureColor( painter, color );
	QPen p = painter.pen();
	p.setWidth( 1 );
	painter.setPen( p );
	painter.drawPie( rect, -40 * 16, 80 * 16 );

	painter.restore();
}


unsigned int PieLegendView::serieCount() const
{
	if ( this->model() != 0 )
		return this->model()->rowCount();
	else
		return 0;
}


QString PieLegendView::serieName( unsigned int serieIdx ) const
{
	return this->model()->headerData( serieIdx, Qt::Vertical ).toString();
}


}
