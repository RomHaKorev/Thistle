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
  along with Marb.  If not, see <http://www.gnu.org/licenses/>.

 Marb  Copyright (C) 2013  Dimitry Ernot
*/

#include "chartstyle.h"

ChartStyle::ChartStyle() {
  myBrush = QBrush( Marb::LightBlue );
  myPen = QPen( QColor( Marb::Blue ), 2 );
  myShape = Marb::Ellipse;
  myType = Marb::Line | Marb::Point;
}


QBrush ChartStyle::brush() const {
  return myBrush;
}


QPen ChartStyle::pen() const {
  return myPen;
}


void ChartStyle::setBrush( const QBrush brush) {
  myBrush = brush;
}

void ChartStyle::setPen( const QPen pen) {
  myPen = pen;
}

void ChartStyle::setShape( const Marb::Shape shape) {
  myShape = shape;
}

void ChartStyle::setType( const Marb::Types type) {
  myType = type;
}


Marb::Shape ChartStyle::shape() const {
  return myShape;
}


Marb::Types ChartStyle::type() const {
  return myType;
}
