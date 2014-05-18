#include "axis.h"
#include "../../kernel/global.h"
#include <QAbstractItemModel>

namespace Thistle {

Axis::Axis() : d_ptr( new AxisPrivate() ) {
    d_ptr->tickSize = 0;
    d_ptr->nbTicks = 10;
    d_ptr->axisPen = QPen( QBrush(Global::Gray), 1.5 );
    d_ptr->tickPen = QPen( QBrush(Global::LightGray), 0 );
    d_ptr->textPen = QPen( QBrush(Global::DarkGray), 1 );
}

Axis::Axis( AxisPrivate* d ) : d_ptr( d ) {
}


/*!Calculates the d_ptr->minimum bounds and the d_ptr->maximum bounds (i.e., the d_ptr->minimum and the d_ptr->maximum displayed on the chart).
Calculates the d_ptr->order and the tick size(delta between two ticks on the Y axis) of the charts values.
If the d_ptr->minimum is equal to the d_ptr->maximum, the d_ptr->minimum bound is equal to d_ptr->minimum - 1 and the d_ptr->maximum bound to d_ptr->maximum + 1
*/
void Axis::calculateBounds() {
    d_ptr->minBound = d_ptr->min;
    d_ptr->maxBound = d_ptr->max;
    if ( d_ptr->maxBound == d_ptr->minBound ) {
        ++d_ptr->maxBound;
        --d_ptr->minBound;
    }
    d_ptr->order = calculateOrder( d_ptr->max - d_ptr->min );
    d_ptr->tickSize = (d_ptr->max - d_ptr->min ) / (d_ptr->nbTicks - 1);
    if ( d_ptr->order >= 10 ) {
        d_ptr->nbDigits = 0;
    } else if ( d_ptr->order == 1 ) {
        d_ptr->nbDigits = 2;
    } else {
        int nbZero = QString::number(d_ptr->order).count( "0" );
        d_ptr->nbDigits = nbZero + 2;
    }
}

long Axis::calculateOrder( qreal value ) const {
    /*Calculates the d_ptr->order of scale for value (i.e. the power of ten greater than value).*/
    long order = 1.0;
    qreal v = abs( value );
    if ( v >= 1 ) {
        while ( v > 1 ) {
            order *= 10.0;
            v /= 10.0;
        }
        order /= 10.0;
    } else if ( v != 0 ) {
        while ( v < 1 ) {
            order /= 10.0;
            v *= 10.0;
        }
        order *= 10.0;
    }
    return order;
}


}