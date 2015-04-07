#ifndef THISTLE_AXISVIEW_H
#define THISTLE_AXISVIEW_H
/*
This file is part of Thistle.

Thistle is free software: you can redistribute it and/or modify
it under the terms of the Lesser GNU General Public License as published by
the Free Software Foundation, either version 3 of the License.

Thistle is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
Lesser GNU General Public License for more details.

You should have received a copy of the Lesser GNU General Public License
along with Thistle.    If not, see <http://www.gnu.org/licenses/>.

Thistle    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/

#include "axischart.h"
//#include "serieformat.h"

#include <QItemDelegate>

#include "linearchart_p.h"

namespace Thistle
{

class DotDelegate;
class BarDelegate;

class CartesianCoordinateSystem;


class ChartView : public AxisChart
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( LinearChart );
public:

    explicit LinearChart(QWidget *parent = 0);

    virtual AbstractChartDelegate* delegate() const
    {
        const Q_D( LinearChart );
        return d->delegate();
    }

protected:
    explicit LinearChart( LinearChartPrivate* d, QWidget *parent = 0);

    QList<int> calculateColumnsOrder() const;
    virtual void updateRects();
    Thistle::Types columnType( int column ) const;
    QList<int> barStyleColumns() const;
    virtual QRectF itemRect( const QModelIndex& index ) const;
    virtual QRectF itemRect( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual void paintSerie( QPainter& painter, int column);
    virtual void paintSerieLegend(QPainter &painter, int column, QPoint pos, int maxHeight) const;
    virtual void paintChart( QPainter& painter );
};

}
#endif // THISTLE_AXISVIEW_H