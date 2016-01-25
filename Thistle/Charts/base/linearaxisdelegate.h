#ifndef THISTLE_LINEARAXISDELEGATE_H
#define THISTLE_LINEARAXISDELEGATE_H

#include <QObject>
#include <QPainter>

#include "private/linearaxisdelegate_p.h"
#include "../base/linearaxis.h"
#include "../../Core/global.h"

namespace Thistle
{
class AbstractCoordinateSystemView;
struct AxisDelegateOptions
{
	Thistle::Layer layer;
	QList<AbstractAxis*> referenceAxis;
	Qt::Alignment labelAlignment;
	QStringList alternativeLabels;
	QPoint labelOffset;

	AxisDelegateOptions(): layer(Thistle::Background), labelAlignment( Qt::AlignLeft ), labelOffset( 0, 0 )
	{}
};

class LinearAxisDelegate
{
public:
	enum TickStyle
	{
		None = 0,
		Short = 1,
		Long = 2,
		Label = 4
	};

	Q_DECLARE_FLAGS( TickStyles, TickStyle )

protected:
	LinearAxisDelegatePrivate* d_ptr;
	inline LinearAxisDelegatePrivate* d_func() const
	{
		return d_ptr;
	}

	virtual void paintBack( QPainter& painter, const LinearAxis& axis, const AxisDelegateOptions& options ) const;
	virtual void paintFront( QPainter& painter, const LinearAxis& axis, const AxisDelegateOptions& options ) const;
	virtual void paintTick( QPainter& painter, const QPointF& pos, qreal angle ) const;
	virtual QRectF paintLabel( QPainter& painter, const QPointF& pos, const QString& label, qreal angle, Qt::Alignment alignment = Qt::AlignLeft, QRectF lastLabelRect = QRectF() ) const;

public:
	LinearAxisDelegate( AbstractCoordinateSystemView* parent = 0 );
	LinearAxisDelegate( const LinearAxisDelegate& other );
	virtual ~LinearAxisDelegate();
	const LinearAxisDelegate& operator=( const LinearAxisDelegate& other );
	void paint( QPainter& painter, const LinearAxis& axis, const AxisDelegateOptions& options ) const;
	virtual qreal calculateTickValue( const LinearAxis& axis ) const;

	QFont font() const;
	QPen  basePen() const;
	QPen  tickPen() const;
	QPen  textPen() const;
	TickStyles tickStyles() const;

	void setFont( const QFont& f );
	void setBasePen( const QPen& p );
	void setTickPen( const QPen& p );
	void setTextPen( const QPen& p );
	void setTickStyles( TickStyles style );
	void setTickReduceFactor( qreal factor );
};
}

Q_DECLARE_OPERATORS_FOR_FLAGS( Thistle::LinearAxisDelegate::TickStyles )
#endif // THISTLE_LINEARAXISDELEGATE_H
