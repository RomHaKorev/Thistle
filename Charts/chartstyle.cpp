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

#include "chartstyle.h"

namespace Marb {

ChartStyle::ChartStyle() : d( new ChartStylePrivate() ) {
    d->brush = QBrush( Color::LightBlue );
    d->pen = QPen( QColor( Color::Blue ), 2 );
    d->shape = Marb::Ellipse;
    d->type = Marb::Line | Marb::Point;
}


QBrush ChartStyle::brush() const {
    return d->brush;
}


QPen ChartStyle::pen() const {
    return d->pen;
}


void ChartStyle::setBrush( const QBrush brush) {
    d->brush = brush;
}

void ChartStyle::setPen( const QPen pen) {
    d->pen = pen;
}

void ChartStyle::setShape( const Marb::Shape shape) {
    d->shape = shape;
}

void ChartStyle::setType( const Marb::Types type) {
    d->type = type;
}


Marb::Shape ChartStyle::shape() const {
    return d->shape;
}


Marb::Types ChartStyle::type() const {
    return d->type;
}

}