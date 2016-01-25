#include "cartesiancoordinateplaneview.h"
#include "private/cartesiancoordplaneview_p.h"
#include "../base/private/abstractcoordinatesystemview_p.h"
#include "private/cartesiancoordinateplaneview_p.h"
#include "linearaxisdelegate.h"
#include "../../Core/global.h"

#include <QFontMetrics>
#include <QAbstractItemModel>
#include <QPainter>
#include <QHeaderView>

namespace Thistle
{
CartesianCoordinatePlaneView::CartesianCoordinatePlaneView()
	: AbstractCoordinateSystemView( new CartesianCoordinatePlane() )
{}


CartesianCoordinatePlaneView::~CartesianCoordinatePlaneView()
{

}


void CartesianCoordinatePlaneView::paintBack( QPainter& painter ) const
{
	Q_UNUSED( painter )
	/* Will be used later */
}


void CartesianCoordinatePlaneView::paintFront( QPainter& painter ) const
{
	const Q_D( CartesianCoordinatePlaneView );
	LinearAxis& x = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::X ) );
	LinearAxis& y = static_cast< LinearAxis& >( d->coordinateSystem()->axis( CartesianCoordinatePlane::Y ) );

	AxisDelegateOptions options;
	options.layer = Thistle::Foreground;

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


QPointF CartesianCoordinatePlaneView::origin() const
{
	return d_ptr->coordinateSystem()->origin();
}


void CartesianCoordinatePlaneView::setRect( const QRect& rect )
{
	d_ptr->rect = rect;
}

}
