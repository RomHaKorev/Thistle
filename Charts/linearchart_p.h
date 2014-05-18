#ifndef LINEARCHART_P_H
#define LINEARCHART_P_H

#include <QRect>
#include <QString>

#include "axischart_p.h"

namespace Thistle {
    class DotDelegate;
    class BarDelegate;
    class OrthogonalAxis;
    class AbstractChartDelegate;

    class LinearChart;

    class LinearChartPrivate : public AxisChartPrivate {
    private:
        QPair<QPointF, QPointF> controlPoints( const QPointF& p0, const QPointF& p1, const QPointF& p2, qreal t = 0.25 ) const;
    protected:
        AbstractChartDelegate* currentDelegate;
        void paintStraightLine( QPainter& painter, int column, bool isActive, bool area = false ) const;
        void paintSpline( QPainter& painter, int column, bool isActive, bool area = false ) const;
        void paintRaw( QPainter& painter, int column, bool isActive ) const;
    public:
        LinearChart* q_func() const {return reinterpret_cast<LinearChart*>(q_ptr);}

        DotDelegate* pointDelegate;
        BarDelegate* barDelegate;
        OrthogonalAxis* orthoAxis;

        LinearChartPrivate( LinearChart* q );
        void selectDelegate( Global::Types type );
        inline AbstractChartDelegate* delegate() const { return currentDelegate; }

        void paintDelegate( QPainter& painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

        void paint( QPainter& painter, int column, Global::Types types, bool active ) const;
    };

}

#endif // LINEARCHART_P_H
