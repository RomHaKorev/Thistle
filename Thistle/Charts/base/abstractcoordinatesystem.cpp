#include "abstractcoordinatesystem.h"
#include "../../kernel/global.h"
#include <QAbstractItemModel>

namespace Thistle
{

AbstractCoordinateSystem::AbstractCoordinateSystem(): d_ptr( 0 )
{
}

AbstractCoordinateSystem::AbstractCoordinateSystem( AbstractCoordinateSystemPrivate* d ): d_ptr( d )
{
  d_ptr->axisPen = QPen( QBrush( Thistle::Colors::Gray ), 1.5 );
  d_ptr->tickPen = QPen( QBrush( Thistle::Colors::LightGray ), 0 );
  d_ptr->textPen = QPen( QBrush( Thistle::Colors::DarkGray ), 1 );
}

AbstractCoordinateSystem::~AbstractCoordinateSystem()
{
	delete d_ptr;
}


QAbstractItemModel* AbstractCoordinateSystem::model() const
{
  return d_ptr->model;
}



const QRectF& AbstractCoordinateSystem::rect() const
{
  return d_ptr->rect;
}


void AbstractCoordinateSystem::setRect( const QRectF& r )
{
  d_ptr->rect = r;
}


void AbstractCoordinateSystem::setModel( QAbstractItemModel* m )
{
  d_ptr->model = m;
}


int AbstractCoordinateSystem::precision( int axisIndex ) const
{
  return d_ptr->axis( axisIndex ).precision();
}


void AbstractCoordinateSystem::setPrecision( int axisIndex, int precision )
{
  d_ptr->axis( axisIndex ).setPrecision( precision );
}

AbstractAxis& AbstractCoordinateSystem::axis( unsigned int index ) const
{
  return d_ptr->axis( index );
}

unsigned int AbstractCoordinateSystem::axisCount() const
{
  return d_ptr->axisCount();
}


qreal AbstractCoordinateSystem::axisLength( int axisIndex ) const
{

	if ( axisIndex < d_ptr->axisCount() )
		return d_ptr->axis( axisIndex ).length();



	/*	return d->xAxis().line().length();
	else if ( axisIndex == CartesianCoordinatePlane::X )
		return d->yAxis().line().length();
	else
		return 0;*/

	return 0;
}

}
