#include "xyaxis.h"
#include <QAbstractItemModel>
#include <QPainter>
#include "../../kernel/global.h"

namespace Thistle
{

XYAxis::XYAxis() : OrthogonalAxis()
{
}


QPointF XYAxis::valueToPoint( const QPointF& point ) const
{
    /*const Q_D( OrthogonalAxis );
    qreal e = qreal( d->maxBound - d->minBound );
    qreal ratio = ( value - d->minBound) / e;
    qreal y = d->yaxis.pointAt( ratio ).y();
    ratio = qreal(axisNumber+0.5) / qreal(d->model->rowCount());
    qreal x = d->xaxis.pointAt( ratio ).x();
    return QPointF( x, y );*/
    return QPointF(point);
}

void XYAxis::calculateBounds()
{
    OrthogonalAxis::calculateBounds();

    Q_D( XYAxis );
    d->xMinBound = d->xMin;
    d->xMaxBound = d->xMax;
    if ( d->xMaxBound == d->xMinBound )
    {
        ++d->xMaxBound;
        --d->xMinBound;
    }
    d->order = calculateOrder( d->xMax - d->xMin );
    d->tickSize = (d->max - d->min ) / (d->nbTicks - 1);
    if ( d->order >= 10 )
    {
        d->nbDigits = 0;
    }
    else if ( d->order == 1 )
    {
        d->nbDigits = 2;
    }
    else
    {
        int nbZero = QString::number( d->order ).count( "0" );
        d->nbDigits = nbZero + 2;
    }
}
}
