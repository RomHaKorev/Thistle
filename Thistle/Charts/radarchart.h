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

#ifndef RADARCHART_H
#define RADARCHART_H

#include "linearchart.h"

#include "radarchart_p.h"

namespace Thistle
{

class RadialAxis;

class RadarChart: public LinearChart
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( RadarChart );
protected:
    virtual QRectF itemRect(const QModelIndex &index) const;
    virtual QPainterPath itemPath( const QModelIndex& index ) const;
    virtual void updateRects();

    RadarChart( RadarChartPrivate* d, QWidget* parent = 0 );

public:
    RadarChart( QWidget* parent = 0 );
    //void process();
};

}

#endif //RADARCHART_H
