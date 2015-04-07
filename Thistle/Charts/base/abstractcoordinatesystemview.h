#ifndef THISTLE_ABSTRACTCOORDSYSVIEW_H
#define THISTLE_ABSTRACTCOORDSYSVIEW_H

#include <QObject>
#include <QPainter>
#include <QRect>
#include <QLineF>
#include <QFont>
#include <QPen>

#include "private/abstractcoordinatesystemview_p.h"
#include "abstractcoordinatesystem.h"

class QAbstractItemModel;

namespace Thistle
{
class AbstractCoordinateSystem;
class AbstractCoordinateSystemViewPrivate;
class LinearAxisDelegate;

class AbstractCoordinateSystemView
{
    Q_DECLARE_PRIVATE( AbstractCoordinateSystemView );
protected:
    AbstractCoordinateSystemViewPrivate* d_ptr;
    AbstractCoordinateSystemView( AbstractCoordinateSystemViewPrivate* d );

public:
    AbstractCoordinateSystemView( AbstractCoordinateSystem* coordSys );
    virtual ~AbstractCoordinateSystemView();

    qreal labelsLength( int axisIndex ) const;
    int   ticksCount( int axisIndex ) const;
    long  order( int axisIndex ) const;

    void setLabelsLength( int axisIndex, int length );
    void setTicksCount( int axisIndex, int nb );
    void setOrder( int axisIndex, long order );

    virtual qreal stepSize() const = 0;

    virtual QPointF origin() const;
    void setRect( const QRect& );

    virtual void paintBack( QPainter& painter ) const = 0;
    virtual void paintFront( QPainter& painter ) const = 0;

    AbstractCoordinateSystem* coordinateSystem() const;

    void setModel( QAbstractItemModel* model );
    QAbstractItemModel* model() const;

    virtual void update() = 0;

  LinearAxisDelegate* delegate() const;
  void setDelegate( LinearAxisDelegate* delegate );

};

}

#endif // THISTLE_ABSTRACTCOORDSYSVIEW_H
