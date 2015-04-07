#include "pointchart_p.h"
#include "../pointchart.h"

#include "../pointchartcoordinateplaneview.h"

namespace Thistle
{

PointChartPrivate::PointChartPrivate( PointChart* q ) : LinearChartPrivate( q, new PointChartCoordinatePlaneView() )
{}


void PointChartPrivate::paintRaw( QPainter& painter, int column, bool isActive ) const
{
	const Q_Q( PointChart );
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
