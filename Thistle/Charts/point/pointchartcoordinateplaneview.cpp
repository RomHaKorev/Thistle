#include "pointchartcoordinateplaneview.h"

namespace Thistle
{

PointChartCoordinatePlaneView::PointChartCoordinatePlaneView()
: CartesianCoordinatePlaneView()
{
	//this->setGridAttributes( AbstractCoordinateSystemView::HorizontalGrid | AbstractCoordinateSystemView::VerticalGrid );
}


PointChartCoordinatePlaneView::~PointChartCoordinatePlaneView()
{}


void PointChartCoordinatePlaneView::update()
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


void PointChartCoordinatePlaneView::paintBack( QPainter& painter ) const
{
	this->paintGrid( painter );

	const Q_D( CartesianCoordinatePlaneView );
	LinearAxis& x = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::X ) );
	LinearAxis& y = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::Y ) );

	AxisDelegateOptions options;
	options.layer = Thistle::Background;
	options.labelAlignment = Qt::AlignLeft;
	options.labelAlignment |= Qt::AlignTop;
	options.referenceAxis.append( &x );
	this->delegate()->paint( painter, y, options );

	options.referenceAxis.clear();
	options.labelAlignment = Qt::AlignBottom;
	options.labelAlignment |= Qt::AlignRight;
	options.referenceAxis.append( &y );
	this->delegate()->paint( painter, x, options );
}


void PointChartCoordinatePlaneView::paintGrid( QPainter& painter ) const
{
	const Q_D( CartesianCoordinatePlaneView );

	LinearAxis& x = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::X ) );
	LinearAxis& y = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::Y ) );

	if ( this->gridAttributes().testFlag( CartesianCoordinatePlaneView::HorizontalGrid ) )
		this->paintGridFor( painter, x, y );

	if ( this->gridAttributes().testFlag( CartesianCoordinatePlaneView::VerticalGrid ) )
		this->paintGridFor( painter, y, x );
	
}


void PointChartCoordinatePlaneView::paintGridFor( QPainter& painter, LinearAxis& axis, LinearAxis& other ) const
{
	const Q_D( CartesianCoordinatePlaneView );

	painter.save();
	painter.setRenderHint( QPainter::Antialiasing, false );
	painter.setPen( d->delegate->tickPen() );

	QPointF p;
	if ( other.line().intersect( axis.line(), &p ) != QLineF::BoundedIntersection )
		p = axis.pinpoint( 0 );

	QPointF offset1 = other.pinpoint( other.minimum() ) - p;
	QPointF offset2 = other.pinpoint( other.maximum() ) - p;

	qreal order = d->delegate->calculateTickValue( axis );
	qreal value = order;
	QPointF pos;
	while ( value <= axis.maximum() )
	{
		pos = axis.pinpoint( value );
		QLineF l( pos + offset2, pos + offset1 );
		painter.drawLine( l );
		value += order;
	}

	value = 0;
	while ( value >= axis.minimum() )
	{
		pos = axis.pinpoint( value );
		QLineF l( pos + offset1, pos + offset2 );
		painter.drawLine( l );
		value -= order;
	}

	painter.restore();
}

}