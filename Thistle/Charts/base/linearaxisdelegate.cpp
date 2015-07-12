#include "linearaxisdelegate.h"

namespace Thistle
{
  LinearAxisDelegate::LinearAxisDelegate( AbstractCoordinateSystemView* parent )
  : d_ptr( new LinearAxisDelegatePrivate( parent ) )
{}


LinearAxisDelegate::~LinearAxisDelegate()
{
  if( d_ptr )
    delete d_ptr;
}


void LinearAxisDelegate::paint( QPainter& painter, const LinearAxis& axis, const AxisDelegateOptions& options ) const
{
  if ( d_ptr->parent == 0 )
    return;
  if ( d_ptr->parent->model() == 0 )
    return;

  switch ( options.layer )
  {
    case Thistle::Foreground:
      this->paintFront( painter, axis, options );
      break;
    case Thistle::Background:
      this->paintBack( painter, axis, options );
      break;
  }
}


void LinearAxisDelegate::paintFront( QPainter& painter, const LinearAxis& axis, const AxisDelegateOptions& options ) const
{
  painter.save();

  qreal angle = 0;
  qreal order = this->calculateTickValue( axis );
  qreal value = order;

  if ( order == 0 )
  {
    qWarning() << Q_FUNC_INFO << "Order is null.";
    return;
  }

  painter.setPen( d_ptr->textPen );
  painter.setFont( d_ptr->font );

  QRectF lastLabelRect;

  if ( options.alternativeLabels.isEmpty() )
  {
    while ( value <= axis.maximum() )
    {
      QPointF pos = axis.pinpoint( value );
      lastLabelRect = this->paintLabel( painter, pos, QString::number( value ), angle, options.labelAlignment, lastLabelRect );
      value += order;
    }

    value = 0;
    while ( value >= axis.minimum() )
    {
      QPointF pos = axis.pinpoint( value );
      lastLabelRect = this->paintLabel( painter, pos, QString::number( value ), angle, options.labelAlignment, lastLabelRect );
      value -= order;
    }
  }
  else
  {
    order = axis.tickIncrement();
    value = axis.minimum();
    Q_FOREACH( QString str, options.alternativeLabels )
    {
      QPointF pos = axis.pinpoint( value );
      lastLabelRect = this->paintLabel( painter, pos, str, angle, options.labelAlignment, lastLabelRect );
      value += order;
    }
  }

  painter.restore();
}


void LinearAxisDelegate::paintBack( QPainter& painter, const LinearAxis& axis, const AxisDelegateOptions& options ) const
{
  painter.save();

  QPointF offset1;
  QPointF offset2;
  qreal angle = axis.line().angle() + 90;

  if ( !options.referenceAxis.isEmpty() )
  {
    LinearAxis* otherAxis = reinterpret_cast< LinearAxis* >( options.referenceAxis.at( 0 ) );

    if ( otherAxis != 0 )
    {
      QLineF referenceTickLine = otherAxis->line();

      QPointF p;
      if ( referenceTickLine.intersect( axis.line(), &p ) != QLineF::BoundedIntersection )
      {
        p = axis.pinpoint( 0 );
      }
      offset1 = otherAxis->pinpoint( otherAxis->minimum() ) - p;
      offset2 = otherAxis->pinpoint( otherAxis->maximum() ) - p;

      if ( !referenceTickLine.isNull() )
        angle = referenceTickLine.angle();
    }
  }

  qreal order = this->calculateTickValue( axis );

  if ( qFuzzyCompare( angle, 0 ) || qFuzzyCompare( angle, 90 ) ) /* Remove Antialiasing if the line is horizontal or vertical */
    painter.setRenderHint( QPainter::Antialiasing, false ); /* The line will not be blurred */

  if ( order == 0 )
  {
    qWarning() << Q_FUNC_INFO << "Order is null.";
    return;
  }

  if ( options.alternativeLabels.isEmpty() )
  {
    qreal value = order;
    QPointF pos;
    while ( value <= axis.maximum() )
    {
      pos = axis.pinpoint( value );
      painter.setPen( d_ptr->base );
      this->paintTick( painter, pos, angle );
      value += order;
    }

    value = 0;
    while ( value >= axis.minimum() )
    {
      pos = axis.pinpoint( value );
      painter.setPen( d_ptr->base );
      this->paintTick( painter, pos, angle );
      value -= order;
    }
  }
  else
  {
    order = axis.tickIncrement();
    qreal value = axis.minimum();
    Q_FOREACH( QString str, options.alternativeLabels )
    {
      QPointF pos = axis.pinpoint( value );
      painter.setPen( d_ptr->base );
      this->paintTick( painter, pos, angle );
      value += order;
    }
  }

  painter.setPen( d_ptr->base );
  painter.drawLine( axis.line() );
  painter.restore();
}


void LinearAxisDelegate::paintTick( QPainter& painter, const QPointF& pos, qreal angle ) const
{
  QLineF line( pos, pos + QPoint( 4, 0 ) );
  line.setAngle( angle );
  QPointF p1( line.p2() );
  line.setAngle( line.angle() + 180 );
  QPointF p2( line.p2() );

  painter.drawLine( p1, p2 );
}


QRectF LinearAxisDelegate::paintLabel( QPainter& painter, const QPointF& pos, const QString& label, qreal angle, Qt::Alignment alignment, QRectF lastLabelRect ) const
{
  Q_UNUSED( angle )

  QFontMetrics metrics( d_ptr->font );
  float h = metrics.height() + 6;
  float w = metrics.width( label ) + 8;

  QRectF r( -w/2.0, -h/2.0, w, h );

  r.translate( pos );

  if ( alignment.testFlag( Qt::AlignTop ) )
    r.translate( 0, -h / 2 );
  else if ( alignment.testFlag( Qt::AlignBottom ) )
    r.translate( 0, h / 2 );

  if ( alignment.testFlag( Qt::AlignLeft ) )
    r.translate( -w/2, 0 );
  else if ( alignment.testFlag( Qt::AlignRight ) )
    r.translate( w/2, 0 );

  if ( r.intersects( lastLabelRect ) )
    return lastLabelRect;

  painter.drawText( r, Qt::AlignCenter, label );

  return r;
}


qreal LinearAxisDelegate::calculateTickValue( const LinearAxis& axis ) const
{
  qreal length = axis.line().length();
  qreal tickSize = length * 0.05;
  qreal percent = tickSize / length;
  qreal value = ( axis.maximum() - axis.minimum() ) * percent;
  qreal round = axis.order() / 2.0;
  qreal floor = value;

  if ( qAbs(floor) < 1.0 ) /* Floor between 0 and 1 */
  {
    qreal exp = qPow( 10, axis.precision() + 1 );
    floor *= exp;
  }

  floor = round * qFloor( ( floor + round / 2 ) / round );

  if ( qAbs( value ) < 1.0 ) /* Floor between 0 and 1 */
  {
    qreal exp = qPow( 10, axis.precision() + 1 );
    floor /= exp;
  }

  if ( floor == 0 )
    floor = ( axis.maximum() - axis.minimum() ) * percent;

  return floor;
}


QFont LinearAxisDelegate::font() const
{
  return d_ptr->font;
}


void LinearAxisDelegate::setFont( const QFont& font )
{
  d_ptr->font = font;
}


QPen LinearAxisDelegate::basePen() const
{
  return d_ptr->base;
}


QPen LinearAxisDelegate::tickPen() const
{
  return d_ptr->tick;
}


QPen LinearAxisDelegate::textPen() const
{
  return d_ptr->textPen;
}


void LinearAxisDelegate::setBasePen( const QPen& p )
{
  d_ptr->base = p;
}


void LinearAxisDelegate::setTickPen( const QPen& p )
{
  d_ptr->tick = p;
}


void LinearAxisDelegate::setTextPen( const QPen& p )
{
  d_ptr->textPen = p;
}


}
