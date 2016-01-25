#include "linechartcoordinateplaneview.h"

#include "../../Core/global.h"


namespace Thistle
{

LineChartCoordinatePlaneView::LineChartCoordinatePlaneView()
	: CartesianCoordinatePlaneView()
{
	Q_D( CartesianCoordinatePlaneView );
	d->coordinateSystem()->setStartOnAxis( true );
}


LineChartCoordinatePlaneView::~LineChartCoordinatePlaneView()
{}


void LineChartCoordinatePlaneView::update()
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
	qreal value = this->model()->index( 0, 0 ).data().toDouble();
	qreal min = 0;
	qreal max = 0;
	for ( int r = 0; r < rows; ++r )
	{
		QString s( this->model()->headerData( r, Qt::Vertical ).toString() );
		textWidth = qMax( textWidth, metrics.width( s ) + 5 );
		for ( int c = 0; c < cols; ++c )
		{
			value = this->model()->index( r, c ).data().toDouble();
			min = float( qMin( min, value ));
			max = float( qMax( max, value ));
			QString s = QString::number( value, 'f', d->coordinateSystem()->precision( CartesianCoordinatePlane::Y ) );
			valueWidth = qMax( valueWidth, metrics.width( s ) );
		}
	}

	QLineF line( d_ptr->rect.bottomLeft() + QPoint( valueWidth, -10 ), d_ptr->rect.topLeft() + QPoint( valueWidth, 10 ) );
	d->coordinateSystem()->setAxisLine( CartesianCoordinatePlane::Y, line );
	QPointF origin = d->coordinateSystem()->valueToPoint( 0, 0 );

	line.setP1( QPoint( line.p1().x(), origin.y() ) );
	line.setP2( line.p1() + QPoint( d_ptr->rect.width() - valueWidth, 0 ) );

	d->coordinateSystem()->setAxisLine( CartesianCoordinatePlane::X, line );
	d->coordinateSystem()->setLabelsLength( CartesianCoordinatePlane::Y, valueWidth + 16 );
	d->coordinateSystem()->setLabelsLength( CartesianCoordinatePlane::X, textWidth + 4 );
	d->coordinateSystem()->setBounds( CartesianCoordinatePlane::Y, min, max );
	d->coordinateSystem()->setBounds( CartesianCoordinatePlane::X, 0, rows );
}


void LineChartCoordinatePlaneView::paintFront( QPainter& painter ) const
{
	const Q_D( CartesianCoordinatePlaneView );
	LinearAxis& x = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::X ) );
	LinearAxis& y = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::Y ) );
	AxisDelegateOptions options;
	options.layer = Thistle::Foreground;
	options.labelAlignment = Qt::AlignLeft;
	options.labelAlignment |= Qt::AlignTop;
	options.referenceAxis.append( &x );
	this->delegate()->paint( painter, y, options );

	if ( d->model != 0 )
	{
		for ( int r = 0; r < d->model->rowCount(); ++r )
			options.alternativeLabels << d->model->headerData( r, Qt::Vertical ).toString();
	}

	qreal size = x.length() / options.alternativeLabels.count();
	options.labelOffset.setX( size / 2.0 );
	options.referenceAxis.clear();
	options.labelAlignment = Qt::AlignBottom;
	options.labelAlignment |= Qt::AlignCenter;
	options.referenceAxis.append( &y );
	this->delegate()->paint( painter, x, options );
}


void LineChartCoordinatePlaneView::paintBack( QPainter& painter ) const
{
	const Q_D( CartesianCoordinatePlaneView );
	LinearAxis& x = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::X ) );
	LinearAxis& y = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::Y ) );
	AxisDelegateOptions options;
	options.layer = Thistle::Background;
	options.labelAlignment = Qt::AlignLeft;
	options.labelAlignment |= Qt::AlignTop;
	options.referenceAxis.append( &x );
	this->delegateForAxis(CartesianCoordinatePlane::Y )->paint( painter, y, options );

	if ( d->model != 0 )
	{
		for ( int r = 0; r < d->model->rowCount(); ++r )
			options.alternativeLabels << d->model->headerData( r, Qt::Vertical ).toString();
	}

	options.referenceAxis.clear();
	options.labelAlignment = Qt::AlignBottom;
	options.labelAlignment |= Qt::AlignRight;
	options.referenceAxis.append( &y );
	this->delegateForAxis(CartesianCoordinatePlane::X )->paint( painter, x, options );
}

}
