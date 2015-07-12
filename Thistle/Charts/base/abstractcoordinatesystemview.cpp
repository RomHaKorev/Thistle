#include "abstractcoordinatesystemview.h"
#include "private/abstractcoordinatesystemview_p.h"

#include "../../kernel/global.h"
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

LinearAxisDelegate* AbstractCoordinateSystemView::delegate() const
{
  return d_ptr->delegate;
}


void AbstractCoordinateSystemView::setDelegate( LinearAxisDelegate* delegate )
{
  d_ptr->delegate = delegate;
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


void AbstractCoordinateSystemView::setGridAttributes( AbstractCoordinateSystemView::GridAttributes attrib )
{
  d_ptr->gridAttrib = attrib;
  this->update();
}


AbstractCoordinateSystemView::GridAttributes AbstractCoordinateSystemView::gridAttributes() const
{
  return AbstractCoordinateSystemView::GridAttributes(d_ptr->gridAttrib);
}

}
