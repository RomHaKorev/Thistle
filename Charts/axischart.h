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
#include "../Marb.h"
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
    QRect myLegendRect;
    QString myTitle;
    QRect myTitleRect;
    QMap<int, ChartStyle> myStyle;

    int myX;
    int myNbDigits;
    int myMinBottomMargin;

    virtual void process();
    virtual void defineRects();
    int scan();
    void calculateBounds();
    void calculateLegendRect();
    qreal calculateOrder( qreal value ) const;
    qreal valueToPx( qreal value) const;
    void setAlphaBeta();
    virtual void resizeEvent( QResizeEvent* ev );
    virtual void paintLegend( QPainter& painter );
    virtual void paintColumnLegend( QPainter& painter, int column, QPoint pos, int maxHeight );
    virtual void paintChart( QPainter& ) = 0;
    virtual void paintEvent(QPaintEvent *event);

    virtual void setScrollBarValues();
    virtual QModelIndex indexAt(const QPoint &point) const;
    virtual QPainterPath itemPath(const QModelIndex &index) const;
    virtual void updateRects() = 0;
    virtual void updateValues();

public:
    explicit AxisChart( QWidget* parent = 0 );
             ~AxisChart();
    Axis* axis() const;
    ChartStyle columnStyle( int column ) const;
    bool save( QString filename );
    void setAxis( Axis* axis );
    void setModel( QAbstractItemModel* model );
    void setColumnStyle( int column, ChartStyle style);
    void setTitle( QString title );
};

#endif // AXISCHART_H
