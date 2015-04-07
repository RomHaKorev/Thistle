#ifndef THISTLE_ABSTRACTAXIS_H
#define THISTLE_ABSTRACTAXIS_H

#include <QtCore>
#include "private/abstractaxis_p.h"

namespace Thistle
{
class AbstractAxisPrivate;

struct AbstractAxis
{
    AbstractAxisPrivate* d_ptr;

    AbstractAxis();
    AbstractAxis( AbstractAxisPrivate* d );
    virtual ~AbstractAxis();

    void setBounds( qreal min, qreal max );
    qreal minimum() const;
    qreal maximum() const;
    int precision() const;
    qreal tickSize() const;
    int ticksCount() const;
    qreal labelsLength() const;
    double order() const;

    void setPrecision( int precision );
    void setTicksCount( int count );
    void setLabelsLength( int length );
    void setTickSize( qreal size );
    void setOrder( double order );

    virtual QPointF pinpoint( qreal value ) const = 0;

    virtual QPointF origin() const = 0;
};

}
#endif // THISTLE_ABSTRACTAXIS_H
