#ifndef AXIS_H
#define AXIS_H

#include <QPainter>
#include <QObject>

namespace Marb {

class QAbstractItemModel;

class Axis : public QObject {
public:
    QRect valuesRect;
    QRect chartRect;
    QFont myFont;
    qreal tickSize;
    QPen axisPen;
    QPen tickPen;
    QPen textPen;
    QAbstractItemModel* myModel;

    int yLabelsLength;
    int xLabelsLength;

    qreal minBound;
    qreal maxBound;
    qreal min;
    qreal max;
    long order;
    int nbDigits;
    int nbTicks;

    Axis( QObject* parent = 0 );
    QFont font() const;
    virtual QPointF origin() const = 0;
    void setModel( QAbstractItemModel* model );
    QAbstractItemModel* model() const;
    virtual void update() = 0;
    virtual QPointF valueToPoint( qreal value, int axisNumber ) const = 0;
    virtual void paintBack( QPainter& painter ) const = 0;
    virtual void paintFront( QPainter& painter ) const = 0;      
    void calculateBounds();
    long calculateOrder( qreal value ) const;
    virtual qreal stepSize() const = 0;
};


#endif // AXIS_H