#include "linearchartcoordinateplaneview.h"

#include "../../kernel/global.h"


namespace Thistle
{

LinearChartCoordinatePlaneView::LinearChartCoordinatePlaneView()
: CartesianCoordinatePlaneView()
{
    Q_D( CartesianCoordinatePlaneView );
    d->coordinateSystem()->setStartOnAxis( true );
}


LinearChartCoordinatePlaneView::~LinearChartCoordinatePlaneView()
{}


void LinearChartCoordinatePlaneView::update()
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
    
    d->coordinateSystem()->setLabelsLength( CartesianCoordinatePlane::Y, valueWidth );
    d->coordinateSystem()->setLabelsLength( CartesianCoordinatePlane::X, textWidth );
    d->coordinateSystem()->setBounds( CartesianCoordinatePlane::Y, min, max );
    d->coordinateSystem()->setBounds( CartesianCoordinatePlane::X, 0, rows );
}


void LinearChartCoordinatePlaneView::paintFront( QPainter& painter ) const
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

	for ( int r = 0; r < d->model->rowCount(); ++r )
	{
		options.alternativeLabels << d->model->headerData( r, Qt::Vertical ).toString();
	}
	options.referenceAxis.clear();
	options.labelAlignment = Qt::AlignBottom;
	options.labelAlignment |= Qt::AlignRight;
	options.referenceAxis.append( &y );
	this->delegate()->paint( painter, x, options );
}


void LinearChartCoordinatePlaneView::paintBack( QPainter& painter ) const
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

	for ( int r = 0; r < d->model->rowCount(); ++r )
	{
		options.alternativeLabels << d->model->headerData( r, Qt::Vertical ).toString();
	}
	options.referenceAxis.clear();
	options.labelAlignment = Qt::AlignBottom;
	options.labelAlignment |= Qt::AlignRight;
	options.referenceAxis.append( &y );
	this->delegate()->paint( painter, x, options );
}


void LinearChartCoordinatePlaneView::paintGrid( QPainter& painter ) const
{
	if ( this->gridAttributes().testFlag( CartesianCoordinatePlaneView::HorizontalGrid ) )
		this->paintGrid_x( painter );

	if ( this->gridAttributes().testFlag( CartesianCoordinatePlaneView::VerticalGrid ) )
		this->paintGrid_y( painter );
}


void LinearChartCoordinatePlaneView::paintGrid_y( QPainter& painter ) const
{
	const Q_D( CartesianCoordinatePlaneView );

	painter.save();
	painter.setRenderHint( QPainter::Antialiasing, false );
	painter.setPen( d->delegate->tickPen() );

	LinearAxis& x = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::X ) );
	LinearAxis& y = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::Y ) );
	
	QPointF p;
	if ( x.line().intersect( y.line(), &p ) != QLineF::BoundedIntersection )
	{
		p = y.pinpoint( 0 );
	}
	QPointF offset1 = x.pinpoint( x.minimum() ) - p;
	QPointF offset2 = x.pinpoint( x.maximum() ) - p;

	qreal order = d->delegate->calculateTickValue( y );
	qreal value = order;
	QPointF pos;
	while ( value <= y.maximum() )
	{
		pos = y.pinpoint( value );
		QLineF l( pos + offset2, pos + offset1 );
		painter.drawLine( l );
		value += order;
	}

	value = 0;
	while ( value >= y.minimum() )
	{
		pos = y.pinpoint( value );
		QLineF l( pos + offset1, pos + offset2 );
		painter.drawLine( l );
		value -= order;
	}

	painter.restore();
}


void LinearChartCoordinatePlaneView::paintGrid_x( QPainter& painter ) const
{
	painter.save();
	painter.setRenderHint( QPainter::Antialiasing, false );
	painter.setPen( d_ptr->delegate->tickPen() );

	const Q_D( CartesianCoordinatePlaneView );
	LinearAxis& x = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::X ) );
	LinearAxis& y = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::Y ) );

	QPointF offset1;
	QPointF offset2;
	qreal angle = y.line().angle();

	QLineF referenceTickLine = y.line();

	QPointF p;
	if ( referenceTickLine.intersect( x.line(), &p ) != QLineF::BoundedIntersection )
	{
		p = x.pinpoint( 0 );
	}
	offset1 = y.pinpoint( y.minimum() ) - p;
	offset2 = y.pinpoint( y.maximum() ) - p;

	if ( !referenceTickLine.isNull() )
		angle = referenceTickLine.angle();

	QPointF pos;
	qreal order = x.tickIncrement();
	qreal value = x.minimum();
	for ( int i = 0; i < d_ptr->model->rowCount(); ++i )
	{
		QPointF pos = x.pinpoint( value );
		QLineF l( pos + offset2, pos + offset1 );
		painter.drawLine( l );
		value += order;
	}
	painter.restore();
}


}
