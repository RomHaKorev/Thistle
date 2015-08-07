#include "kiviatcoordinatesystemview.h"
#include "kiviatcoordinatesystem.h"
#include "../base/linearaxisdelegate.h"

#include <QFontMetrics>

namespace Thistle
{

KiviatCoordinateSystemView::KiviatCoordinateSystemView()
  //: AbstractCoordinateSystemView( new KiviatCoordinateSystem() )
  : AbstractCoordinateSystemView( new KiviatCoordinateSystem() )
{
	this->delegate()->setTickReduceFactor( 2 );
}

KiviatCoordinateSystemView::~KiviatCoordinateSystemView()
{}

void KiviatCoordinateSystemView::paintBack( QPainter& painter ) const
{

  QRectF clipRect = painter.clipBoundingRect();
  AxisDelegateOptions options;
  options.layer = Thistle::Background;
  options.labelAlignment = Qt::AlignLeft;
  options.labelAlignment |= Qt::AlignTop;

  unsigned int count = d_ptr->coordinateSystem()->axisCount();

  QFontMetrics metrics( d_ptr->font );
  painter.setFont( d_ptr->font );

  for ( unsigned int index = 0; index < count; ++index )
  {
    LinearAxis& a = static_cast< LinearAxis& >( d_ptr->coordinateSystem()->axis( index ) );
    this->delegate()->paint( painter, a, options );
	QPointF p = a.line().p2();
	QPointF origin = a.line().p1();
	QString text = a.name();

	QRectF r( 0, 0, metrics.width( text ) + 8, metrics.height() + 4 );
	r.translate( p );

	if ( p.x() < origin.x() )
		r.translate( -r.width(), 0 );
	if ( p.y() < origin.y() )
		r.translate( 0, -r.height() );

	painter.drawText( r, Qt::AlignLeft, text );

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
