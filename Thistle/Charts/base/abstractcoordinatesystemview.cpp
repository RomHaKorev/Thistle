#include "abstractcoordinatesystemview.h"
#include "private/abstractcoordinatesystemview_p.h"

#include "../../Core/global.h"
#include "linearaxisdelegate.h"

#include <QFontMetrics>
#include <QAbstractItemModel>
#include <QPainter>

namespace Thistle
{

AbstractCoordinateSystemView::AbstractCoordinateSystemView( AbstractCoordinateSystem* coordSys )
	: d_ptr( new AbstractCoordinateSystemViewPrivate( coordSys, this ) )
{}

AbstractCoordinateSystemView::AbstractCoordinateSystemView( AbstractCoordinateSystemViewPrivate* d )
	: d_ptr( d )
{}

AbstractCoordinateSystemView::~AbstractCoordinateSystemView()
{
	if ( d_ptr )
		delete d_ptr;
}

void AbstractCoordinateSystemView::setModel( QAbstractItemModel* model )
{
	d_ptr->model = model;
	this->update();
}


QAbstractItemModel* AbstractCoordinateSystemView::model() const
{
	return d_ptr->model;
}

QSharedPointer<LinearAxisDelegate> AbstractCoordinateSystemView::delegate() const
{
	return d_ptr->defaultDelegate;
}


void AbstractCoordinateSystemView::setDelegate( QSharedPointer<LinearAxisDelegate> delegate )
{
	d_ptr->defaultDelegate = delegate;
}


void AbstractCoordinateSystemView::setDelegateForAxis( int index, QSharedPointer<LinearAxisDelegate> delegate )
{
	d_ptr->setDelegateFor( index, delegate );
}


QSharedPointer<LinearAxisDelegate> AbstractCoordinateSystemView::delegateForAxis( int index ) const
{
	return d_ptr->delegateFor( index );
}


QPointF AbstractCoordinateSystemView::origin() const
{
	return d_ptr->coordinateSystem()->origin();
}


void AbstractCoordinateSystemView::setRect( const QRect& rect )
{
	d_ptr->rect = rect;
}


AbstractCoordinateSystem* AbstractCoordinateSystemView::coordinateSystem() const
{
	return d_ptr->coordinateSystem();
}

}
