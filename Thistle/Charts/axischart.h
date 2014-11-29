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

#ifndef THISTLE_AXISCHART_H
#define THISTLE_AXISCHART_H

#include <QAbstractItemView>
#include "../kernel/global.h"
#include "serieformat.h"

#include "abstractchart.h"
#include "axischart_p.h"

namespace Thistle
{
class PointChart;
class RadialChart;
class AxisView;


class AxisChart : public AbstractChart
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( AxisChart );
protected:
    virtual void defineRects();
    void scan();

    virtual QList<int> calculateColumnsOrder() const;

    virtual void paintSerieLegend( QPainter& painter, int serie, QPoint pos, int metricsH ) const;
    virtual void paintChart( QPainter& ) = 0;

    virtual void setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command );

    AxisChart( AxisChartPrivate* d, QWidget* parent = 0 );
public:

    explicit AxisChart( QWidget* parent = 0 );
    ~AxisChart();
    AxisView* axisView() const;
    
    void setAxisView( AxisView* axis );
    void setModel( QAbstractItemModel* model );
};

}

#endif // THISTLE_AXISCHART_H
