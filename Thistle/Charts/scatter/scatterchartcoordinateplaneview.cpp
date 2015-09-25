#include "scatterchartcoordinateplaneview.h"

namespace Thistle
{

ScatterChartCoordinatePlaneView::ScatterChartCoordinatePlaneView()
	: CartesianCoordinatePlaneView()
{
	//this->setGridAttributes( AbstractCoordinateSystemView::HorizontalGrid | AbstractCoordinateSystemView::VerticalGrid );
}


ScatterChartCoordinatePlaneView::~ScatterChartCoordinatePlaneView()
{}


void ScatterChartCoordinatePlaneView::update()
{
	Q_D( CartesianCoordinatePlaneView );
	if ( d_ptr->model == 0 )
	{
		return;
	}

	int rows = this->model()->rowCount();
	int cols = this->model()->columnCount();
	QFontMetrics metrics( this->delegate()->font() );
	int textWidth = 0;
	int valueWidth = 0;
	QPointF value = this->model()->index( 0, 0 ).data().toPointF();
	QPointF min = QPointF( 0, 0 );
	QPointF max = QPointF( 0, 0 );

	for ( int r = 0; r < rows; ++r )
	{
		QString s( this->model()->headerData( r, Qt::Vertical ).toString() );
		textWidth = qMax( textWidth, metrics.width( s ) + 5 );
		for ( int c = 0; c < cols; ++c )
		{
			value = this->model()->index( r, c ).data().toPointF();

			min.setX( float( qMin( min.x(), value.x() ) ) );
			max.setX( float( qMax( max.x(), value.x() ) ) );

			min.setY( float( qMin( min.y(), value.y() ) ) );
			max.setY( float( qMax( max.y(), value.y() ) ) );

			QString s = QString::number( value.y(), 'f', d->coordinateSystem()->precision( CartesianCoordinatePlane::Y ) );
			valueWidth = qMax( valueWidth, metrics.width( s ) );
		}
	}

	d->coordinateSystem()->setLabelsLength( CartesianCoordinatePlane::Y, valueWidth );
	d->coordinateSystem()->setLabelsLength( CartesianCoordinatePlane::X, textWidth );
	d->coordinateSystem()->setBounds( CartesianCoordinatePlane::Y, min.y(), max.y() );
	d->coordinateSystem()->setBounds( CartesianCoordinatePlane::X, min.x(), max.x() );

	QLineF lineY( d_ptr->rect.bottomLeft() + QPoint( valueWidth, -10 ), d_ptr->rect.topLeft() + QPoint( valueWidth, 10 ) );
	QLineF lineX( d_ptr->rect.topLeft() + QPoint( valueWidth, 10 ), d_ptr->rect.topLeft() + QPoint( d_ptr->rect.width() - valueWidth, 10 ) );

	d->coordinateSystem()->setAxisLine( CartesianCoordinatePlane::Y, lineY );
	d->coordinateSystem()->setAxisLine( CartesianCoordinatePlane::X, lineX );

	QPointF origin = d->coordinateSystem()->valueToPoint( 0, 0 );

	lineY.setP1( QPointF( origin.x(), lineY.p1().y() ) );
	lineY.setP2( QPointF( origin.x(), lineY.p2().y() ) );
	lineX.setP1( QPointF( lineX.p1().x(), origin.y() ) );
	lineX.setP2( QPointF( lineX.p2().x(), origin.y() ) );

	d->coordinateSystem()->setAxisLine( CartesianCoordinatePlane::Y, lineY );
	d->coordinateSystem()->setAxisLine( CartesianCoordinatePlane::X, lineX );
}


void ScatterChartCoordinatePlaneView::paintBack( QPainter& painter ) const
{
	const Q_D( CartesianCoordinatePlaneView );
	LinearAxis& x = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::X ) );
	LinearAxis& y = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::Y ) );

	AxisDelegateOptions options;
	options.layer = Thistle::Background;
	options.labelAlignment = Qt::AlignLeft;
	options.labelAlignment |= Qt::AlignTop;
	options.referenceAxis.append( &x );
	this->delegateForAxis( CartesianCoordinatePlane::Y )->paint( painter, y, options );

	options.referenceAxis.clear();
	options.labelAlignment = Qt::AlignBottom;
	options.labelAlignment |= Qt::AlignRight;
	options.referenceAxis.append( &y );
	this->delegateForAxis( CartesianCoordinatePlane::X )->paint( painter, x, options );
}

}
