#ifndef ABSTRACTCHART_H
#define ABSTRACTCHART_H

#include "../kernel/abstractitemview.h"

#include <QWidget>

namespace Thistle {

    class ChartLegend;
    class AbstractChartPrivate;
    /*!
    \class AbstractChart
    \brief The AbstractChart class provides an abstract base for every chart view.

    The AbstractChart class provides the basic functionality for every chart view. It is a abstract class and cannot be instancied itself.
    \a AbstractChart provides a standard interface based on \c Thistle::AbstractItemView. It reimplements basic logic for calculating and analizing the model data.

    \sa
    Thistle::AbstractItemView
    */
    class AbstractChart :  public AbstractItemView {
        Q_DECLARE_PRIVATE( AbstractChart )
    protected:
        virtual void process();

        /*!
        This pure virtual function defines the \c QRect used to to draw the different elements composing the view (series, legend, title, etc.).
        */
        virtual void defineRects() = 0;
        virtual void scan() = 0;
        void calculateLegendRect( const QRect& source );

        virtual void setScrollBarValues();
        virtual QPainterPath itemPath(const QModelIndex& index) const;
        virtual void updateRects() = 0;

        virtual void resizeEvent( QResizeEvent* ev );
        virtual void paintEvent(QPaintEvent* event);
        //virtual void paintLegend( QPainter& painter ) const;
        //virtual void paintSerieLegend( QPainter& painter, int serie, QPoint pos, int maxHeight ) const = 0;
        virtual void paintChart( QPainter& painter ) = 0;

        AbstractChart( AbstractChartPrivate* d, QWidget* parent = 0 );

        /*void mousePressEvent( QMouseEvent* ev );
        void mouseReleaseEvent( QMouseEvent* ev );*/

    public:
        AbstractChart( QWidget* parent = 0 );
        ~AbstractChart();

        virtual QModelIndex indexAt(const QPoint& point) const;

        void setTitle( const QString& title );

        const QFont& titleFont() const;
        void setTitleFont( const QFont& font );

        void setLegend( ChartLegend* legend );
        ChartLegend* legend() const;

        public slots:
            void updateValues();
    };

}

#endif //ABSTRACTCHART_H
