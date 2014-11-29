#ifndef THISTLE_LINEARAXIS_H
#define THISTLE_LINEARAXIS_H

#include <QtCore>

namespace Thistle
{
class LinearAxisPrivate;
/*
    qreal minBound;
    qreal maxBound;
    qreal min;
    qreal max;
    long order;
    int nbDigits;
    int nbTicks;
    qreal tickSize;
    qreal labelsLength;*/
struct LinearAxis
{
    LinearAxisPrivate* d_ptr;
    LinearAxis();

    void setBounds( qreal min, qreal max );
    qreal minimum() const;
    qreal maximum() const;
    int precision() const;
    qreal tickSize() const;
    int ticksCount() const;
    qreal labelsLength() const;
    long order() const;

    void setPrecision( int precision );
    void setTicksCount( int count );
    void setLabelsLength( int length );
    void setTickSize( qreal size );
    void setOrder( long order );
    void setLine( const QLineF& line );

    QPointF pinpoint( qreal value ) const;

    QPointF origin() const;

    const QLineF& line() const;


};

}
#endif // THISTLE_LINEARAXIS_H