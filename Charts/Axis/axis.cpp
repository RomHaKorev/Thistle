#include "axis.h"
#include "../../kernel/Marb.h"
#include <QAbstractItemModel>

Axis::Axis( QObject* parent ) : QObject( parent ) {
    valuesRect = QRect();
    chartRect = QRect();
    myFont = QFont();
    tickSize = 0;
    nbTicks = 10;
    axisPen = QPen( QBrush(Marb::Gray), 1.5 );
    tickPen = QPen( QBrush(Marb::LightGray), 0 );
    textPen = QPen( QBrush(Marb::DarkGray), 1 );
}


void Axis::calculateBounds() {
    /*Calculates the minimum bounds and the maximum bounds (i.e., the minimum and the maximum displayed on the chart).
    Calculates the order and the tick size(delta between two ticks on the Y axis) of the charts values.
    If the minimum is equal to the maximum, the minimum bound is equal to minimum - 1 and the maximum bound to maximum + 1*/
    minBound = min;
    maxBound = max;
    if ( maxBound == minBound ) {
        ++maxBound;
        --minBound;
    }
    order = calculateOrder( max - min );
    tickSize = (max - min ) / (nbTicks - 1);
    if ( order >= 10 ) {
        nbDigits = 0;
    } else if ( order == 1 ) {
        nbDigits = 2;
    } else {
        int nbZero = QString::number(order).count( "0" );
        nbDigits = nbZero + 2;
    }
}

long Axis::calculateOrder( qreal value ) const {
    /*Calculates the order of scale for value (i.e. the power of ten greater than value).*/
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

QFont Axis::font() const {
    return myFont;
}

QAbstractItemModel* Axis::model() const {
    return myModel;
}

void Axis::setModel( QAbstractItemModel* model ) {
    myModel = model;
}
