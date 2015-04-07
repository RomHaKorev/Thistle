#include "pointchartcoordinateplaneview.h"

namespace Thistle
{

PointChartCoordinatePlaneView::PointChartCoordinatePlaneView()
: CartesianCoordinatePlaneView()
{}


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


#if 0
void PointChartCoordinatePlaneView::paintXAxis( QPainter& painter ) const
{
	const Q_D( CartesianCoordinatePlaneView );
	painter.save();
	QFontMetrics metrics( d_ptr->font );
	int h = metrics.height();
	qreal order = d->coordinateSystem()->tickSize( CartesianCoordinatePlane::X );

	qreal precision = d->coordinateSystem()->precision( CartesianCoordinatePlane::X );

	painter.setPen( d_ptr->textPen );

	qreal max = d->coordinateSystem()->maximum( CartesianCoordinatePlane::X );
	qreal min = d->coordinateSystem()->minimum( CartesianCoordinatePlane::X );

	if ( ( max / order ) < 8 ) /* Too few of ticks while we have place */
		order /= 2;

	qreal x = order;

	while ( x < max )
	{
		this->paintXAxisTick( painter, x );
		x += order;
	}

	x = -order;

	while ( x >= min )
	{
		this->paintXAxisTick( painter, x );
		x -= order;
	}

	/* Paints the axis line */
	painter.setPen( d_ptr->axisPen );
	painter.drawLine( d->coordinateSystem()->axisLine( CartesianCoordinatePlane::X ) );

	painter.restore();
}

void PointChartCoordinatePlaneView::paintXAxisFront( QPainter& painter ) const
{
    const Q_D( CartesianCoordinatePlaneView );
    painter.save();
    QFontMetrics metrics( d_ptr->font );
    int h = metrics.height();
    qreal order = d->coordinateSystem()->tickSize( CartesianCoordinatePlane::X );
    
	qreal precision = d->coordinateSystem()->precision( CartesianCoordinatePlane::X );

    painter.setPen( d_ptr->textPen );

	qreal max = d->coordinateSystem()->maximum( CartesianCoordinatePlane::X );
	qreal min = d->coordinateSystem()->minimum( CartesianCoordinatePlane::X );

	qreal labelSize = metrics.width( QString::number( max, 'f', precision ) + "X" ) + 10; /* Size needed to draw the label*/
	qreal tickSize = d->coordinateSystem()->valueToPoint( order, 0 ).x() - d->coordinateSystem()->valueToPoint( 0, 0 ).x(); /* Size between two ticks */
	int nbLabel = qMax( 1, qFloor( labelSize / tickSize + 0.5 ) ); /* Number of labels we have to skip to avoid overlays */
	qreal step = order * nbLabel; /* Value of the increment */

	if ( ( max / step ) < 8 && ( tickSize > labelSize * 2 ) ) /* Too few of ticks while we have place */
		step /= 2;
	
	qreal x = step;

	while ( x < max )
	{
		QPoint p1( d->coordinateSystem()->valueToPoint( x, 0 ).x(), d->coordinateSystem()->origin().y() );
		QString s = QString::number( x, 'f', precision );
		QRect r( QPoint( p1.x(), p1.y() ), QSize( metrics.width( s ), h ) );
		painter.drawText( r, Qt::AlignRight, s );
		x += step;
	}
	
	x = -step;

	while ( x >= min )
	{
		QPoint p1( d->coordinateSystem()->valueToPoint( x, 0 ).x(), d->coordinateSystem()->origin().y() );
		QString s = QString::number( x, 'f', precision );
		QRect r( QPoint( p1.x(), p1.y() ), QSize( metrics.width( s ), h ) );
		painter.drawText( r, Qt::AlignRight, s );
		x -= step;
	}
	painter.restore();
}
#endif

}