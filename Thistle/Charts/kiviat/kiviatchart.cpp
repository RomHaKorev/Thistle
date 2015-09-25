#include "kiviatchart.h"
#include "kiviatcoordinatesystemview.h"

namespace Thistle {

KiviatChart::KiviatChart( QWidget* parent )
//: SerieChart( new KiviatCoordinateSystemView() )
	: LinearChart( new KiviatChartPrivate( this, new KiviatCoordinateSystemView() ), parent )
{

}

KiviatChart::~KiviatChart()
{}


QPainterPath KiviatChart::itemPath( const QModelIndex& index ) const
{
	QPainterPath path;

	if ( !index.isValid() )
		return path;

	AbstractCoordinateSystem* coordSys = this->coordinateSystemView()->coordinateSystem();
	if ( index.row() >= int(coordSys->axisCount()) ) // index.row could be equal to -1
		return path;

	AbstractAxis& axis = coordSys->axis( index.row() );

	bool ok;
	double value = index.data().toDouble( &ok );
	QVariant v = index.data();

	if ( !ok )
		return path;

	QPointF p = axis.pinpoint( value );

	path.addRect( QRectF( -4, -4, 8, 8 ).translated( p ).normalized() );
	return path;
}

#if 0
void KiviatChart::paintEvent( QPaintEvent* ev )
{
	QPainter painter( this->viewport() );
	painter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );

	coordinateSystemView()->paintBack( painter );
	for ( int column = 0; column < this->model()->columnCount(); ++column )
	{
		this->paintSerie( painter, column );
	}
	coordinateSystemView()->paintFront( painter );
}
void KiviatChart::paintSerie( QPainter& painter, int column )
{
	painter.save();

	SerieFormat style = this->serieFormat( column );
	painter.setBrush( style.brush() );
	painter.setPen( style.pen() );

	for ( int row = 0; row < this->model()->rowCount(); ++row )
	{
		QRectF r = this->itemRect( row, column );
		painter.drawRect( r );
	}

	painter.restore();
}
#endif


} // namespace Thistle
