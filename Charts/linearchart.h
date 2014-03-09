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

#ifndef LINEARCHART_H
#define LINEARCHART_H

#include "axischart.h"
#include "chartstyle.h"

#include <QItemDelegate>

class PointDelegate;
class BarDelegate;

class OrthogonalAxis;

class LinearChart : public AxisChart {
    Q_OBJECT
public:
    explicit LinearChart(QWidget *parent = 0);
protected:
    PointDelegate* myPointDelegate;
    BarDelegate* myBarDelegate;
    bool myVerticalXAxis;
    OrthogonalAxis* myOrthoAxis;

    QList<int> calculateColumnsOrder() const;
    virtual void updateRects();
    Marb::Types columnType( int column ) const;
    QList<int> barStyleColumns() const;
    virtual QRectF itemRect( const QModelIndex& index ) const;
    void paintValues( QPainter& painter, int column);
    virtual void paintColumnLegend(QPainter &painter, int column, QPoint pos, int maxHeight);
    virtual void paintChart( QPainter& painter );
};
#endif // LINEARCHART_H
