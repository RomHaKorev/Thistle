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

#ifndef PIECHART_H
#define PIECHART_H

#include "axischart.h"



class AbstractChart;

class PieChart : public AbstractChart {
    Q_OBJECT
    Q_PROPERTY(qreal startAngle READ startAngle WRITE setStartAngle)
public:
    explicit PieChart( QWidget* parent = 0 );

    QRect visualRect( const QModelIndex& index ) const;

    void scrollTo( const QModelIndex& index, ScrollHint hint = EnsureVisible );
    void setSplitted( bool splitted = true );
    void setRing( bool ring = true );
    virtual void defineRects();

protected:
    QRect     myRect;
    qreal     myTotal;
    bool        mySplitted;
    bool        myRing;
    qreal     myStartAngle;

    qreal startAngle() const;

    int scan();

    virtual void configureColor( QPainter& painter, QColor base, int flag ) const;

    virtual QPainterPath itemPath( const QModelIndex& index ) const;
    virtual QPainterPath itemPart( qreal angle, qreal delta, bool splitted = false ) const;

    virtual void calculateLegendRect( const QRect& source );
    virtual void paintChart( QPainter& painter ) const;
    virtual void paintLegend( QPainter& painter) const;
    virtual void paintSerieLegend( QPainter& painter, int serie, QPoint pos, int maxHeight ) const;
    virtual void paintPart( QPainter& painter, qreal angle, qreal delta, QColor color, bool isSelected = false ) const;
    virtual void paintPartSplitted( QPainter& painter, qreal angle, qreal delta, QColor color, bool isSelected = false ) const;

    //void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command);
    virtual QPointF splittedOffset( qreal angle, qreal delta ) const;
    //virtual void updateChart();
    virtual void updateRects();


public slots:
    void setStartAngle( qreal angle );
};

#endif // PIECHART_H
