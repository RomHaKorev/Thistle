#ifndef THISTLE_LINEARAXISDELEGATE_H
#define THISTLE_LINEARAXISDELEGATE_H

#include <QObject>
#include <QPainter>

#include "private/linearaxisdelegate_p.h"
#include "../base/linearaxis.h"
#include "abstractcoordinatesystemview.h"
#include "../../kernel/global.h"

namespace Thistle
{
  struct AxisDelegateOptions
  {
    Thistle::Layer layer;
    QList<AbstractAxis*> referenceAxis;
    Qt::Alignment labelAlignment;
    QStringList alternativeLabels;

    AxisDelegateOptions(): layer(Thistle::Background), labelAlignment( Qt::AlignLeft )
    {}
  };

  class LinearAxisDelegate
  {
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
    LinearAxisDelegate( AbstractCoordinateSystemView* parent );
    virtual ~LinearAxisDelegate();

    void paint( QPainter& painter, const LinearAxis& axis, const AxisDelegateOptions& options ) const;

	virtual qreal calculateTickValue( const LinearAxis& axis ) const;

    QFont font() const;
    QPen  basePen() const;
    QPen  tickPen() const;
    QPen  textPen() const;
    void setFont( const QFont& f );
    void setBasePen( const QPen& p );
    void setTickPen( const QPen& p );
    void setTextPen( const QPen& p );
  };
}
#endif // THISTLE_LINEARAXISDELEGATE_H
