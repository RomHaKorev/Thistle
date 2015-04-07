#include "kiviatcoordinatesystemview.h"
#include "kiviatcoordinatesystem.h"
#include "../base/linearaxisdelegate.h"

namespace Thistle
{

KiviatCoordinateSystemView::KiviatCoordinateSystemView()
  //: AbstractCoordinateSystemView( new KiviatCoordinateSystem() )
  : AbstractCoordinateSystemView( new KiviatCoordinateSystem() )
{}

KiviatCoordinateSystemView::~KiviatCoordinateSystemView()
{}

void KiviatCoordinateSystemView::paintBack( QPainter& painter ) const
{
  AxisDelegateOptions options;
  options.layer = Thistle::Background;
  options.labelAlignment = Qt::AlignLeft;
  options.labelAlignment |= Qt::AlignTop;

  unsigned int count = d_ptr->coordinateSystem()->axisCount();
  for ( unsigned int index = 0; index < count; ++index )
  {
    LinearAxis& a = static_cast< LinearAxis& >( d_ptr->coordinateSystem()->axis( index ) );
    this->delegate()->paint( painter, a, options );
  }

//	painter.drawRect( d_ptr->rect );
//	painter.drawRect( d_ptr->coordinateSystem()->rect() );
}

void KiviatCoordinateSystemView::paintFront( QPainter& painter ) const
{
  Q_UNUSED( painter )
}

void KiviatCoordinateSystemView::update()
{
  d_ptr->coordinateSystem()->setRect( d_ptr->rect );
  d_ptr->coordinateSystem()->update( d_ptr->model );
}

} // namespace Thistle
