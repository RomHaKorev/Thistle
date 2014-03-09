#ifndef ORTHOGONALAXIS_H
#define ORTHOGONALAXIS_H

#include "axis.h"

#include <QObject>
#include <QPainter>
#include <QRect>
#include <QLineF>
#include <QFont>
#include <QPen>

class QAbstractItemModel;

class OrthogonalAxis : public Axis {
public:
    QLineF myYaxis;
    QLineF myXaxis;
    qreal xLabelsLength;
    qreal yLabelsLength;
    bool myStartOnAxis;
    bool myVerticalLabels;


protected:
    void paintXAxis( QPainter& painter ) const;
    void paintYAxis( QPainter& painter ) const;
public:
    OrthogonalAxis();
    
    void calculateBounds();
    qreal calculateOrder( qreal value ) const;
    QFont font() const;
    QAbstractItemModel* model() const;
    virtual QPointF origin() const;
    //void paint( QPainter& painter ) const;
    void paintBack( QPainter& painter ) const;
    void paintFront( QPainter& painter ) const;
    void scanModel();
    void setModel( QAbstractItemModel* model );
    void setstartOnAxis( bool startOn );
    bool startOnAxis() const;
    qreal stepSize() const;
    QPointF valueToPoint( qreal value, int axisNumber ) const;

public slots:
    void update();

};

#endif // ORTHOGONALAXIS_H
