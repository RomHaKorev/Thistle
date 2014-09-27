#ifndef AXIS_P_H
#define AXIS_P_H

#include <QRect>
#include <QFont>
#include <QAbstractItemModel>
#include <QPen>

namespace Thistle
{

struct AbstractAxisPrivate
{
    QRect valuesRect;
    QRect chartRect;
    QFont font;
    qreal tickSize;
    QPen axisPen;
    QPen tickPen;
    QPen textPen;
    QAbstractItemModel* model;

    qreal xLabelsLength;
    qreal yLabelsLength;

    qreal minBound;
    qreal maxBound;
    qreal min;
    qreal max;
    long order;
    int nbDigits;
    int nbTicks;

    AbstractAxisPrivate();
};

}

#endif // AXIS_P_H
