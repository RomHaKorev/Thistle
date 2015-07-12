#include "cartesiancoordinateplaneview.h"
#include "private/cartesiancoordplaneview_p.h"
#include "../base/private/abstractcoordinatesystemview_p.h"
#include "private/cartesiancoordinateplaneview_p.h"
#include "linearaxisdelegate.h"
#include "../../kernel/global.h"

#include <QFontMetrics>
#include <QAbstractItemModel>
#include <QPainter>
#include <QHeaderView>

namespace Thistle
{
CartesianCoordinatePlaneView::CartesianCoordinatePlaneView()
: AbstractCoordinateSystemView( new CartesianCoordinatePlane() )
{}


void CartesianCoordinatePlaneView::paintBack( QPainter& painter ) const
{}

void CartesianCoordinatePlaneView::paintGrid( QPainter& painter ) const
{}


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
  this->delegate()->paint( painter, y, options );

  options.referenceAxis.clear();
  options.labelAlignment = Qt::AlignBottom;
  options.labelAlignment |= Qt::AlignRight;
  options.referenceAxis.append( &y );
  this->delegate()->paint( painter, x, options );
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
