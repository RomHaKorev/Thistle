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

#ifndef THISTLE_LINECHART_H
#define THISTLE_LINECHART_H

#include "../base/seriechart.h"
//#include "serieformat.h"

#include <QItemDelegate>

#include "private/linechart_p.h"

namespace Thistle
{

class DotDelegate;
class BarDelegate;

class CartesianCoordinatePlane;


class LineChart : public SerieChart
{
	Q_OBJECT
    Q_DECLARE_PRIVATE( LineChart );
public:

    explicit LineChart(QWidget *parent = 0);
    explicit LineChart( AbstractCoordinateSystemView* coordSysView, QWidget *parent = 0 );

	virtual AbstractChartDelegate* delegate() const
	{
        const Q_D( LineChart );
		return d->delegate();
	}

protected:
    explicit LineChart( LineChartPrivate* d, QWidget *parent = 0);
    explicit LineChart( LineChartPrivate* d, AbstractCoordinateSystemView* coordSysView, QWidget* parent = 0 );
	virtual QPainterPath itemPath( const QModelIndex& index ) const;
};

}

#endif // THISTLE_LINECHART_H
