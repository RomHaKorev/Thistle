#include "PieLegendView.h"
#include "piechart.h"

#include <QDebug>

namespace Thistle
{
PieLegendView::PieLegendView( PieChart* parent ): AbstractLegendView( parent ), chart( parent )
{}


PieLegendView::~PieLegendView()
{}


void PieLegendView::paintSerie( QPainter &painter, int serie, const QRect& rect ) const
{
	QPoint posText = rect.topLeft() + QPoint( 15, 0 );
	QString s( chart->model()->headerData( serie, Qt::Vertical ).toString() );
	painter.drawText( posText, s );
	painter.save();
	QColor color( chart->model()->data( chart->model()->index( serie, 0 ), Qt::DecorationRole ).toString() );
	if ( !color.isValid() )
	{
		color = Colors::predefinedColor( serie );
	}
	chart->configureColor( painter, color );
	painter.drawRect( rect );
	painter.restore();
}
}
