#include "scatterchart_p.h"
#include "../scatterchart.h"

#include "../scatterchartcoordinateplaneview.h"

namespace Thistle
{

ScatterChartPrivate::ScatterChartPrivate( ScatterChart* q ) : LineChartPrivate( q, new ScatterChartCoordinatePlaneView() )
{}


void ScatterChartPrivate::paintRaw( QPainter& painter, int column, bool isActive ) const
{
	const Q_Q( ScatterChart );
	int rows = q->model()->rowCount();
	for ( int r = 0; r < rows; ++r )
	{
		QModelIndex index = q->model()->index( r, column );
		QStyleOptionViewItem option;
		QPointF value = index.data().toPointF();

		if ( value.y() < 0 )
			option.decorationPosition = QStyleOptionViewItem::Bottom;
		else
			option.decorationPosition = QStyleOptionViewItem::Top;

		if ( isActive == false )
			option.state = QStyle::State_Off;
		else
			option.state = QStyle::State_Selected;

		QRectF rect = q->itemRect( index );
		option.rect = rect.toRect();

		this->paintDelegate( painter, option, index );
	}
}

}
