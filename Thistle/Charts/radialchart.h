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

#ifndef RADIALCHART_H
#define RADIALCHART_H
#include "axischart.h"

#include "radialchart_p.h"

namespace Thistle
{

class RadialAxis;

class RadialChart: public AxisChart
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( RadialChart );
protected:
    QRectF itemRect(const QModelIndex &index) const;
    QPainterPath itemPath( const QModelIndex& index ) const;
    void updateRects();
    void paintSerie( QPainter& painter, int column ) const;
    void paintAxis( QPainter& painter ) const;
    void paintTicks( QPainter& painter ) const;
    virtual void paintChart( QPainter& );

    RadialChart( RadialChartPrivate* d, QWidget* parent = 0 );

public:
    RadialChart( QWidget* parent = 0 );
    //void process();
};

}

#endif //RADIALCHART_H
