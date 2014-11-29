#ifndef THISTLE_COORDINATESYSTEM_H
#define THISTLE_COORDINATESYSTEM_H

#include <QPainter>
#include <QObject>

#include "abstractcoordinatesystem_p.h"

class QAbstractItemModel;

namespace Thistle
{

class AbstractCoordinateSystem
{
protected:
    AbstractCoordinateSystemPrivate* d_ptr;
    AbstractCoordinateSystem( AbstractCoordinateSystemPrivate* d );
public:
    AbstractCoordinateSystem();

    virtual QRect& valuesRect();
    QRect& chartRect();
    
    void setValuesRect( const QRect& r );
    void setChartRect( const QRect& r );

    QAbstractItemModel* model() const;
    void setModel( QAbstractItemModel* m );

    QFont font() const;
    QPen  axisPen() const;
    QPen  tickPen() const;
    QPen  textPen() const;
    void setFont( const QFont& f );
    void setAxisPen( const QPen& p );
    void setTickPen( const QPen& p );
    void setTextPen( const QPen& p );

    virtual QPointF origin() const = 0;
    virtual void    update() = 0;
    virtual QPointF valueToPoint( qreal value, int axisNumber ) const = 0;
    virtual qreal   stepSize() const = 0;

    virtual void paintBack( QPainter& painter ) const = 0;
    virtual void paintFront( QPainter& painter ) const = 0;
   
    //virtual void calculateMetrics() = 0;
};

}

#endif // THISTLE_COORDINATESYSTEM_H
