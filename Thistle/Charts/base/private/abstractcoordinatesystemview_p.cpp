#include "abstractcoordinatesystemview_p.h"
#include "../abstractcoordinatesystem.h"
#include "../../../Core/global.h"

#include <QAbstractItemModel>

namespace Thistle
{
AbstractCoordinateSystemViewPrivate::AbstractCoordinateSystemViewPrivate( AbstractCoordinateSystem* coordSys, AbstractCoordinateSystemView* coordSysView )
	: tickPen( Qt::lightGray, 1 ), model( 0 ), defaultDelegate( new LinearAxisDelegate( coordSysView ) ), coordSystem( coordSys )
{}


AbstractCoordinateSystemViewPrivate::~AbstractCoordinateSystemViewPrivate()
{
	if ( coordSystem )
		delete coordSystem;
}


AbstractCoordinateSystem* AbstractCoordinateSystemViewPrivate::coordinateSystem() const
{
	return coordSystem;
}


void AbstractCoordinateSystemViewPrivate::setDelegateFor( int index, QSharedPointer<LinearAxisDelegate> delegate )
{
	if ( delegates.contains( index ) )
		delegates[ index ] = delegate;
	else
		delegates.insert( index, delegate );
}


QSharedPointer<LinearAxisDelegate> AbstractCoordinateSystemViewPrivate::delegateFor( int index )
{
	if ( delegates.contains( index ) )
		if ( !delegates[ index ].isNull() )
			return delegates[ index ];

	return defaultDelegate;
}

}
