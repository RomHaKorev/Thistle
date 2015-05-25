#include "kiviatcoordinatesystem.h"

namespace Thistle {

KiviatCoordinateSystem::KiviatCoordinateSystem()
	: AbstractCoordinateSystem( new KiviatCoordinateSystemPrivate() )
{}

KiviatCoordinateSystem::~KiviatCoordinateSystem()
{}

QPointF KiviatCoordinateSystem::origin() const
{
	return d_ptr->rect.center();
}

void KiviatCoordinateSystem::setRect( const QRectF& r )
{
	qreal length = qMin( r.width(), r.height() );
	QRectF r1( -length / 2.0, -length / 2.0, length, length );
	r1.translate( r.center() );

	d_ptr->rect = r1;
}


void KiviatCoordinateSystem::update( QAbstractItemModel* model )
{
	Q_D( KiviatCoordinateSystem );
	if ( model == 0 )
	{
		return;
	}

	d->update( model, d->rect );

	/*QLineF line( d->valuesRect.bottomLeft() - QPoint( 0, 10 ), d->valuesRect.topLeft() + QPoint( 0, 10 ) );
	this->setAxisLine( CartesianCoordinatePlane::Y, line );
	QPointF origin = this->valueToPoint( 0, 0 );

	line.setP1( QPoint( line.p1().x(), origin.y() ) );
	line.setP2( line.p1() + QPoint( d->valuesRect.width(), 0 ) );

	this->setAxisLine( CartesianCoordinatePlane::X, line );

	d->verticalLabels = ( this->stepSize() <= d->yAxis().labelsLength() );
	if ( d->verticalLabels == true )
	{
		qreal delta = qAbs( d->chartRect.bottom() - origin.y() );
		if ( delta < d->yAxis().labelsLength() )
		{
			d->valuesRect.setHeight( d->valuesRect.height() - ( d->yAxis().labelsLength() - delta ) );
			this->update();
		}
	}*/
}

} // namespace Thistle
