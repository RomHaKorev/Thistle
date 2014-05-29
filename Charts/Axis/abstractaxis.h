#ifndef AXIS_H
#define AXIS_H

#include <QPainter>
#include <QObject>

#include "abstractaxis_p.h"

class QAbstractItemModel;

namespace Thistle {

    /*!
    \class AbstractAxis
    \brief The AbstractAxis class provides an abstract base for every charts axis.

    The AbstractAxis class provides the basic functionality for every axis. It is a abstract class and cannot be instancied itself.
    \a AbstractAxis provides a standard interface to define item position and axis painting. It reimplements basic logic for positionning items in the view.

    \sa
    Thistle::OrthogonalAxis and Thistle::RadialAxis
    */
    class AbstractAxis {
    protected:
        AbstractAxisPrivate* d_ptr;
        AbstractAxis( AbstractAxisPrivate* d );
    public:    
        AbstractAxis();

        /*!
        Returns the bounding rect where the items should be painted.
        */
        QRect& valuesRect() { return d_ptr->valuesRect; };

        /*!
        Returns the bounding rect where the whole chart should be painted.
        */
        QRect& chartRect() { return d_ptr->chartRect; };

        /*!
        Returns the font using to write labels on axis.
        */
        QFont font() const { return d_ptr->font; };

        /*!
        Returns the tick size on Y-axis.
        */
        qreal tickSize() const { return d_ptr->tickSize; };
        /*!
        Returns the \a QPen used to paint the axis.
        */
        QPen axisPen() const { return d_ptr->axisPen; };

        /*!
        Returns the \a QPen used to paint the ticks.
        */
        QPen tickPen() const { return d_ptr->tickPen; };
        
        /*!
        Returns the \a QPen used to write labels.
        */
        QPen textPen() const { return d_ptr->textPen; };

        /*!
        Returns the \a QAbstractItemModel used by the chart.
        */
        QAbstractItemModel* model() const { return d_ptr->model; };

        /*!
        Returns the maximum length allowed to paint the labels on the X-axis.
        */
        qreal xLabelsLength() const { return d_ptr->xLabelsLength; };
        
        /*!
        Returns the maximum length in pixels allowed to paint the labels on the Y-axis.
        */
        qreal yLabelsLength() const { return d_ptr->yLabelsLength; };

        /*!
        Returns the minimum bound for model data.
        */
        qreal minBound() const { return d_ptr->minBound; };

        /*!
        Returns the maximum bound for model data.
        */
        qreal maxBound() const { return d_ptr->maxBound; };

        /*!
        Returns the minimum value in the model data.
        */
        qreal min() const { return d_ptr->min; };

        /*!
        Returns the maximum value in the model data.
        */
        qreal max() const { return d_ptr->max; };

        /*!
        Returns the values order.
        */
        long order() const { return d_ptr->order; };

        /*!
        Returns the number of digits allowed to paint the Y-axis values.
        */
        int nbDigits() const { return d_ptr->nbDigits; };

        /*!
        Returns the number of ticks.
        */
        int nbTicks() const { return d_ptr->nbTicks; };

        /*!
        Sets the bounding rect where the items should be painted.
        */
        void setValuesRect( const QRect& r ) { d_ptr->valuesRect = r; };

        /*!
        Sets the bounding rect where the whole chart should be painted.
        */
        void setChartRect( const QRect& r ) { d_ptr->chartRect = r; };

        /*!
        Sets the \a QFont used to write the labels.
        */
        void setFont( const QFont& f ) { d_ptr->font = f; };

        /*!
        Sets the tick size on Y-axis.
        */
        void setTickSize( qreal t ) { if( t >= 0 ) d_ptr->tickSize = t; };

        /*!
        Sets the \a QPen used to draw axis.
        */
        void setAxisPen( const QPen& p ) { d_ptr->axisPen = p; };

        /*!
        Sets the \a QPen used to draw the ticks.
        */
        void setTickPen( const QPen& p ) { d_ptr->tickPen = p; };
        
        /*!
        Sets the \a QPen used to write labels.
        */
        void setTextPen( const QPen& p ) { d_ptr->textPen = p; };

        /*!
        Sets the \a QAbstractItemModel to use. The model should be the same than the chart view.
        */
        void setModel( QAbstractItemModel* m ) { d_ptr->model = m; };

        /*!
        Sets the maximum length allowed to paint the labels on the X-axis.
        */
        void setXLabelsLength( int l ) { d_ptr->xLabelsLength = l; };

        /*!
        Sets the maximum length allowed to paint the labels on the X-axis.
        */
        void setYLabelsLength( int l ) { d_ptr->yLabelsLength = l; };

        /*!
        Sets the minimum bound of the model data.
        */
        void setMinBound( qreal b ) { d_ptr->minBound = b; };

        /*!
        Sets the maximum bound of the model data.
        */
        void setMaxBound( qreal b ) { d_ptr->maxBound = b; };

        /*!
        Sets the minimum value in the model data.
        */
        void setMin( qreal b ) { d_ptr->min = b; };

        /*!
        Sets the maximum value in the model data.
        */
        void setMax( qreal b ) { d_ptr->max = b; };

        /*!
        Sets the order of the model data.
        */
        void setOrder( long o ) { d_ptr->order = o; };

        /*!
        Sets the number of digits for the labels on the Y axis.
        */
        void setNbDigits( int n ) { d_ptr->nbDigits = n; };

        /*!
        Sets the mumber of ticks to paint.
        */
        void setNbTicks( int n ) { d_ptr->nbTicks = n; };


        /*!
        Returns the axis origin.
        */
        virtual QPointF origin() const = 0;

        /*!
        Ask to the axis for painting.
        */
        virtual void update() = 0;

        /*!
        Returns the position in the view of \a value relative to the \a axisNumber (used by the chart using multiple axis such as \a RadarChart).
        */
        virtual QPointF valueToPoint( qreal value, int axisNumber ) const = 0;

        /*!
        Paints the part of the axis in the background of the chart such as ticks.
        */
        virtual void paintBack( QPainter& painter ) const = 0;

        /*!
        Paints the part of the axis in the foreground of the chart such as labels.
        */
        virtual void paintFront( QPainter& painter ) const = 0;      

        /*!
        Calculates the model data bounds.
        \sa
        minBounds(), maxBounds, min(), max(), setMinBounds(), setMaxBounds, setMin() and setMax()
        */
        void calculateBounds();

        /*!
        Calculates the order of the given \a value.
        */
        long calculateOrder( qreal value ) const;

        /*!
        Returns the step size on the X axis.
        */
        virtual qreal stepSize() const = 0;
    };

}

#endif // AXIS_H