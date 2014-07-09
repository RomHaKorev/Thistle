#ifndef AXIS_H
#define AXIS_H

#include <QPainter>
#include <QObject>

#include "abstractaxis_p.h"

class QAbstractItemModel;

namespace Thistle {

    class AbstractAxis {
    protected:
        AbstractAxisPrivate* d_ptr;
        AbstractAxis( AbstractAxisPrivate* d );
    public:    
        AbstractAxis();

        QRect& valuesRect() { return d_ptr->valuesRect; };
        QRect& chartRect() { return d_ptr->chartRect; };

        QFont font() const { return d_ptr->font; };
        qreal tickSize() const { return d_ptr->tickSize; };
        QPen axisPen() const { return d_ptr->axisPen; };
        QPen tickPen() const { return d_ptr->tickPen; };
        QPen textPen() const { return d_ptr->textPen; };

        QAbstractItemModel* model() const { return d_ptr->model; };

        qreal xLabelsLength() const { return d_ptr->xLabelsLength; };        
        qreal yLabelsLength() const { return d_ptr->yLabelsLength; };

        qreal minBound() const { return d_ptr->minBound; };
        qreal maxBound() const { return d_ptr->maxBound; };
        qreal min() const { return d_ptr->min; };
        qreal max() const { return d_ptr->max; };

        long order() const { return d_ptr->order; };
        int nbDigits() const { return d_ptr->nbDigits; };
        int nbTicks() const { return d_ptr->nbTicks; };

        void setValuesRect( const QRect& r ) { d_ptr->valuesRect = r; };
        void setChartRect( const QRect& r ) { d_ptr->chartRect = r; };

        void setFont( const QFont& f ) { d_ptr->font = f; };
        void setTickSize( qreal t ) { if( t >= 0 ) d_ptr->tickSize = t; };
        void setAxisPen( const QPen& p ) { d_ptr->axisPen = p; };

        void setTickPen( const QPen& p ) { d_ptr->tickPen = p; };
        void setTextPen( const QPen& p ) { d_ptr->textPen = p; };

        void setModel( QAbstractItemModel* m ) { d_ptr->model = m; };
        
        void setXLabelsLength( int l ) { d_ptr->xLabelsLength = l; };
        void setYLabelsLength( int l ) { d_ptr->yLabelsLength = l; };

        void setMinBound( qreal b ) { d_ptr->minBound = b; };
        void setMaxBound( qreal b ) { d_ptr->maxBound = b; };
        void setMin( qreal b ) { d_ptr->min = b; };
        void setMax( qreal b ) { d_ptr->max = b; };

        void setOrder( long o ) { d_ptr->order = o; };
        void setNbDigits( int n ) { d_ptr->nbDigits = n; };
        void setNbTicks( int n ) { d_ptr->nbTicks = n; };

        virtual QPointF origin() const = 0;
        virtual void update() = 0;
        virtual QPointF valueToPoint( qreal value, int axisNumber ) const = 0;

        virtual void paintBack( QPainter& painter ) const = 0;
        virtual void paintFront( QPainter& painter ) const = 0;      

        void calculateBounds();
        long calculateOrder( qreal value ) const;

        virtual qreal stepSize() const = 0;
    };

}

#endif // AXIS_H