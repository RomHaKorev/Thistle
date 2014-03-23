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

#ifndef CHARTSTYLE_H
#define CHARTSTYLE_H

#include <QBrush>
#include <QPen>
#include "../kernel/Marb.h"



class ChartStyle {
protected:
    QBrush myBrush;
    QPen myPen;
    Marb::Shape myShape;
    Marb::Types myType;

public:
    ChartStyle();
    void setBrush( const QBrush brush);
    void setPen( const QPen pen);
    void setShape( const Marb::Shape shape);
    void setType( const Marb::Types type);
    QBrush brush() const;
    QPen pen() const;
    Marb::Shape shape() const;
    Marb::Types type() const;
};


#endif // CHARTSTYLE_H
