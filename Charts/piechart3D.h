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

#ifndef PIECHART3D_H
#define PIECHART3D_H

#include "piechart.h"

class PieChart3D : public PieChart {
    Q_OBJECT

public:
    enum Render {
        Plain = 0,
        WireFrame = 1,
        Translucent = 2
    };

    Q_ENUMS( Render )

    explicit PieChart3D(QWidget *parent = 0);
    void setRender( PieChart3D::Render );
protected:
    QList<qreal> myAngles;
    PieChart3D::Render myRender;
    QPainterPath myFront;
    qreal myHeight;
    virtual void updateChart();
    virtual QPainterPath itemSidesPath( const QModelIndex& index ) const;
    virtual QPainterPath itemExternalPart( qreal angle, qreal delta, bool splitted = false ) const;
    virtual QPainterPath side( qreal angle, QPointF centerOffset, bool splitted ) const;
    virtual void configureColor(QPainter &painter, QColor base, int flag) const;

    void paintEvent(QPaintEvent *event);
    virtual void paintSides( QPainter& painter );
    virtual void paintExternal( QPainter& painter, bool top );
    virtual void paintLeft( QPainter& painter, QColor color );
};

#endif // PIECHART3D_H
