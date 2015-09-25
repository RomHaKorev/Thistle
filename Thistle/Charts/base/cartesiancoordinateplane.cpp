#include "cartesiancoordinateplane.h"
#include "private/cartesiancoordinateplane_p.h"
#include "../../kernel/global.h"

#include <QFontMetrics>
#include <QAbstractItemModel>
#include <QPainter>

namespace Thistle
{

CartesianCoordinatePlane::CartesianCoordinatePlane() : AbstractCoordinateSystem( new CartesianCoordinatePlanePrivate() )
{}

CartesianCoordinatePlane::CartesianCoordinatePlane( CartesianCoordinatePlanePrivate* d ) : AbstractCoordinateSystem( d )
{}

CartesianCoordinatePlane::~CartesianCoordinatePlane()
{}


QPointF CartesianCoordinatePlane::origin() const
{
	const Q_D( CartesianCoordinatePlane );
	return d->xAxis().origin();
}


void CartesianCoordinatePlane::setStartOnAxis( bool startOn )
{
	Q_D( CartesianCoordinatePlane );
	if ( startOn )
		d->valueOffset.setX( 0.5 );
	else
		d->valueOffset.setX( 0 );
}


bool CartesianCoordinatePlane::startOnAxis() const
{
	const Q_D( CartesianCoordinatePlane );
	return d->valueOffset.x() != 0;
}


void CartesianCoordinatePlane::update( QAbstractItemModel* model )
{
	Q_UNUSED( model )

	Q_D( CartesianCoordinatePlane );
	if ( d->model == 0 )
	{
		return;
	}

	QLineF line( d->rect.bottomLeft() - QPoint( 0, 10 ), d->valuesRect.topLeft() + QPoint( 0, 10 ) );
	this->setAxisLine( CartesianCoordinatePlane::Y, line );
	QPointF origin = this->valueToPoint( 0, 0 );

	line.setP1( QPoint( line.p1().x(), origin.y() ) );
	line.setP2( line.p1() + QPoint( d->valuesRect.width(), 0 ) );

	this->setAxisLine( CartesianCoordinatePlane::X, line );

	qreal stepSize = line.length() / d->model->rowCount();

	d->verticalLabels = ( stepSize <= d->yAxis().labelsLength() );
	if ( d->verticalLabels == true )
	{
		qreal delta = qAbs( d->rect.bottom() - origin.y() );
		if ( delta < d->yAxis().labelsLength() )
		{
			d->valuesRect.setHeight( d->valuesRect.height() - ( d->yAxis().labelsLength() - delta ) );
			this->update();
		}
	}
}


QPointF CartesianCoordinatePlane::valueToPoint( qreal x, qreal y ) const
{
	const Q_D( CartesianCoordinatePlane );
	x = d->xAxis().pinpoint( x  + d->valueOffset.x() ).x();
	y = d->yAxis().pinpoint( y + d->valueOffset.y() ).y();
	return QPointF( x, y );
}


qreal CartesianCoordinatePlane::labelsLength( CartesianCoordinatePlane::Axis axis ) const
{
	const Q_D( CartesianCoordinatePlane );
	switch( axis )
	{
		case CartesianCoordinatePlane::X:
		return d->xAxis().labelsLength();
		break;
		default:
		return d->yAxis().labelsLength();
	}

};

qreal CartesianCoordinatePlane::tickIncrement( CartesianCoordinatePlane::Axis axis ) const
{
	const Q_D( CartesianCoordinatePlane );
	switch( axis )
	{
		case CartesianCoordinatePlane::X:
		return d->xAxis().tickIncrement();
		break;
		default:
		return d->yAxis().tickIncrement();
	}

};

qreal CartesianCoordinatePlane::minimum( CartesianCoordinatePlane::Axis axis ) const
{
	const Q_D( CartesianCoordinatePlane );
	switch( axis )
	{
		case CartesianCoordinatePlane::X:
		return d->xAxis().minimum();
		break;
		default:
		return d->yAxis().minimum();
	}

};


qreal CartesianCoordinatePlane::maximum( CartesianCoordinatePlane::Axis axis ) const
{
	const Q_D( CartesianCoordinatePlane );
	switch( axis )
	{
		case CartesianCoordinatePlane::X:
		return d->xAxis().maximum();
		break;
		default:
		return d->yAxis().maximum();
	}

};

long CartesianCoordinatePlane::order( CartesianCoordinatePlane::Axis axis ) const
{
	const Q_D( CartesianCoordinatePlane );

	switch( axis )
	{
		case CartesianCoordinatePlane::X:
		return d->xAxis().order();
		break;
		default:
		return d->yAxis().order();
	}

};


int CartesianCoordinatePlane::ticksCount( CartesianCoordinatePlane::Axis axis ) const
{
	const Q_D( CartesianCoordinatePlane );
	switch( axis )
	{
		case CartesianCoordinatePlane::X:
		return d->xAxis().ticksCount();
		break;
		default:
		return d->yAxis().ticksCount();
	}

};



void CartesianCoordinatePlane::setTickIncrement( CartesianCoordinatePlane::Axis axis, qreal size )
{
	Q_D( CartesianCoordinatePlane );
	if( size <= 0 )
		return;

	switch( axis )
	{
		case CartesianCoordinatePlane::X:
			d->xAxis().setTickIncrement( size );
		break;
		default:
			d->yAxis().setTickIncrement(  size );
	}
};

void CartesianCoordinatePlane::setLabelsLength( CartesianCoordinatePlane::Axis axis, int length )
{
	Q_D( CartesianCoordinatePlane );
	switch( axis )
	{
		case CartesianCoordinatePlane::X:
			d->xAxis().setLabelsLength( length );
		break;
		default:
			d->yAxis().setLabelsLength( length );
	}
};

void CartesianCoordinatePlane::setBounds( CartesianCoordinatePlane::Axis axis, qreal minimum, qreal maximum )
{
	Q_D( CartesianCoordinatePlane );
	switch( axis )
	{
		case CartesianCoordinatePlane::X:
			d->xAxis().setBounds( minimum, maximum );
		break;
		default:
			d->yAxis().setBounds( minimum, maximum );
	}
};


void CartesianCoordinatePlane::setOrder( CartesianCoordinatePlane::Axis axis, long order )
{
	Q_D( CartesianCoordinatePlane );
	switch( axis )
	{
		case CartesianCoordinatePlane::X:
			d->xAxis().setOrder( order );
		break;
		default:
			d->yAxis().setOrder( order );
	}
};


void CartesianCoordinatePlane::setTicksCount( CartesianCoordinatePlane::Axis axis, int nb )
{
	Q_D( CartesianCoordinatePlane );
	switch( axis )
	{
		case CartesianCoordinatePlane::X:
			d->xAxis().setTicksCount( nb );
		break;
		default:
			d->yAxis().setTicksCount( nb );
	}
};


void CartesianCoordinatePlane::setAxisLine( CartesianCoordinatePlane::Axis axis, const QLineF& line )
{
	Q_D( CartesianCoordinatePlane );
	switch( axis )
	{
		case CartesianCoordinatePlane::X:
			d->xAxis().setLine( line );
		break;
		default:
			d->yAxis().setLine( line );
	}
}


const QLineF& CartesianCoordinatePlane::axisLine(CartesianCoordinatePlane::Axis axis )
{
	Q_D( CartesianCoordinatePlane );
	switch( axis )
	{
		case CartesianCoordinatePlane::X:
		return d->xAxis().line();
		break;
		default:
		return d->yAxis().line();
	}
}

}
