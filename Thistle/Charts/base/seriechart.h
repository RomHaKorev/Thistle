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

#ifndef THISTLE_SERIECHART_H
#define THISTLE_SERIECHART_H

#include <QAbstractItemView>
#include "../../kernel/global.h"
#include "serieformat.h"

#include "abstractchart.h"
#include "private/seriechart_p.h"

namespace Thistle
{
class PointChart;
class RadialChart;
class CartesianCoordinatePlaneView;


class SerieChart : public AbstractChart
{
    Q_OBJECT
    Q_DECLARE_PRIVATE( SerieChart );
protected:
    virtual void scan();
	virtual void updateRects();

    SerieChart( SerieChartPrivate* d, QWidget* parent = 0 );

	virtual void paintSerie( QPainter& painter, int column );
	virtual void paintEvent( QPaintEvent* ev );

	virtual QList<int> calculateColumnsOrder() const;
	Thistle::Types columnType( int column ) const;
	QList<int> barStyleColumns() const;
	bool isActiveColumn( int column ) const;

public:
    explicit SerieChart( AbstractCoordinateSystemView* coordSysView, QWidget* parent = 0 );
    ~SerieChart();
    
    void setCoordSysView( AbstractCoordinateSystemView* axis );
    inline AbstractCoordinateSystemView* coordinateSystemView() const
    {
        const Q_D( SerieChart );
        return d->coordinateSystemView();
    }

	void paint( QPainter& painter );

    void setModel( QAbstractItemModel* model );
};

}

#endif // THISTLE_SERIECHART_H
