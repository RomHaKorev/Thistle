/*
 This file is part of Marb.

    Marb is free software: you can redistribute it and/or modify
    it under the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    Marb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with Marb.    If not, see <http://www.gnu.org/licenses/>.

 Marb    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/

#ifndef AXISCHART_H
#define AXISCHART_H

#include <QAbstractItemView>
#include "../kernel/Marb.h"
#include "chartstyle.h"

#include "abstractchart.h"



class PointChart;
class RadialChart;
class Axis;

class AxisChart : public AbstractChart {
    Q_OBJECT
private:
    Axis* myAxis;
protected:
    QPointF myOrigin;
    qreal myMin;
    qreal myMax;
    qreal myMinBound;
    qreal myMaxBound;
    qreal myOrder;
    int myNbTicks;
    qreal myTickSize;
    QMap<int, ChartStyle> myStyle;

    int myX;
    int myNbDigits;

    virtual void defineRects();
    int scan();
    void calculateBounds();
    qreal calculateOrder( qreal value ) const;

    virtual void paintSerieLegend( QPainter& painter, int serie, QPoint pos, int metricsH ) const;
    virtual void paintChart( QPainter& ) const = 0;
    virtual void paintEvent(QPaintEvent *event);

    //virtual void updateValues();

public:
    explicit AxisChart( QWidget* parent = 0 );
             ~AxisChart();
    Axis* axis() const;
    ChartStyle columnStyle( int column ) const;

    void setAxis( Axis* axis );
    void setModel( QAbstractItemModel* model );
    void setColumnStyle( int column, ChartStyle style);
};


#endif // AXISCHART_H
