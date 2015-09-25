#include "linearaxisdelegate.h"
#include "abstractcoordinatesystemview.h"

namespace Thistle
{
LinearAxisDelegate::LinearAxisDelegate( AbstractCoordinateSystemView* parent )
	: d_ptr( new LinearAxisDelegatePrivate( parent ) )
{
	d_ptr->tickStyle = LinearAxisDelegate::Short | LinearAxisDelegate::Label;
}


LinearAxisDelegate::LinearAxisDelegate( const LinearAxisDelegate& other ): d_ptr( new LinearAxisDelegatePrivate( 0 ) )
{
	d_ptr->parent = other.d_ptr->parent;
	d_ptr->font = other.d_ptr->font;
	d_ptr->base = other.d_ptr->base;
	d_ptr->tick = other.d_ptr->tick;
	d_ptr->textPen = other.d_ptr->textPen;
	d_ptr->tickReduceFactor = other.d_ptr->tickReduceFactor;
	d_ptr->tickStyle = other.d_ptr->tickStyle;
}


const LinearAxisDelegate& LinearAxisDelegate::operator=( const LinearAxisDelegate& other )
{
	d_ptr->parent = other.d_ptr->parent;
	d_ptr->font = other.d_ptr->font;
	d_ptr->base = other.d_ptr->base;
	d_ptr->tick = other.d_ptr->tick;
	d_ptr->textPen = other.d_ptr->textPen;
	d_ptr->tickReduceFactor = other.d_ptr->tickReduceFactor;
	d_ptr->tickStyle = other.d_ptr->tickStyle;
	return *this;
}

LinearAxisDelegate::~LinearAxisDelegate()
{
	if ( d_ptr )
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
	if ( !TickStyles( d_ptr->tickStyle ).testFlag( LinearAxisDelegate::Label ) )
		return;

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

	TickStyles styles( d_ptr->tickStyle );
	if ( styles == LinearAxisDelegate::None )
		return;

	painter.setPen( d_ptr->base );
	QPointF offset1;
	QPointF offset2;
	QPointF center;
	qreal angle = axis.line().angle() + 90;
	QLineF referenceTickLine;

	// RK: Need refactoring...
	if ( !options.referenceAxis.isEmpty() )
	{
		LinearAxis* otherAxis = reinterpret_cast< LinearAxis* >( options.referenceAxis.at( 0 ) );

		if ( otherAxis != 0 )
		{
			referenceTickLine = otherAxis->line();
			if ( referenceTickLine.intersect( axis.line(), &center ) != QLineF::BoundedIntersection )
				center = axis.pinpoint( 0 );
			offset1 = otherAxis->pinpoint( otherAxis->minimum() );
			offset2 = otherAxis->pinpoint( otherAxis->maximum() );
			if ( !referenceTickLine.isNull() )
				angle = referenceTickLine.angle();
		}
	}

	referenceTickLine.setAngle( angle );
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
			if ( styles.testFlag( LinearAxisDelegate::Long ) )
			{
				painter.setPen( d_ptr->tick );
				QPointF p = pos - center;
				painter.drawLine( offset1 + p, offset2 + p );
				painter.setPen( d_ptr->base );
			}
			if ( styles.testFlag( LinearAxisDelegate::Short ) | styles.testFlag( LinearAxisDelegate::Long ) )
				this->paintTick( painter, pos, angle );
			value += order;
		}

		value = 0;
		while ( value >= axis.minimum() )
		{
			pos = axis.pinpoint( value );
			if ( styles.testFlag( LinearAxisDelegate::Long ) )
			{
				painter.setPen( d_ptr->tick );
				QPointF p = pos - center;
				painter.drawLine( offset1 + p, offset2 + p );
				painter.setPen( d_ptr->base );
			}
			if ( styles.testFlag( LinearAxisDelegate::Short ) | styles.testFlag( LinearAxisDelegate::Long ) )
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
			if ( styles.testFlag( LinearAxisDelegate::Long ) )
			{
				painter.setPen( d_ptr->tick );
				QPointF p = pos - center;
				painter.drawLine( offset1 + p, offset2 + p );
				painter.setPen( d_ptr->base );
			}
			if ( styles.testFlag( LinearAxisDelegate::Short ) )
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

	return floor * d_ptr->tickReduceFactor;
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

void LinearAxisDelegate::setTickStyles( LinearAxisDelegate::TickStyles style )
{
	d_ptr->tickStyle = style;
}

LinearAxisDelegate::TickStyles LinearAxisDelegate::tickStyles() const
{
	TickStyles styles( d_ptr->tickStyle );
	return styles;
	/*switch( d_ptr->tickStyle )
	{
		case LinearAxisDelegate::Short:
			return LinearAxisDelegate::Short;
		case LinearAxisDelegate::Long:
			return LinearAxisDelegate::Long;
		default:
			return LinearAxisDelegate::None;
	}*/
}


void LinearAxisDelegate::setTickReduceFactor( qreal factor )
{
	if ( factor >= 0 )
		d_ptr->tickReduceFactor = factor;
}


}
