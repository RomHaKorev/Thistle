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

#include "serieformat.h"

namespace Thistle {

SerieFormat::SerieFormat() : d( new SerieFormatPrivate() ) {
    d->brush = QBrush( Global::LightBlue );
    d->pen = QPen( QColor( Global::Blue ), 2 );
    d->shape = Global::Ellipse;
    d->type = Global::Line | Global::Dot;
}

/*!
Returns the \c QBrush used to draw the items (point and bar ).
*/
QBrush SerieFormat::brush() const {
    return d->brush;
}


/*!
Returns the \c QPen used to draw the items
*/
QPen SerieFormat::pen() const {
    return d->pen;
}


/*!
Sets the \c QBrush used to draw the items (point and bar ).
*/
void SerieFormat::setBrush( const QBrush brush) {
    d->brush = brush;
}


/*!
Sets the \c QPen used to draw the items
*/
void SerieFormat::setPen( const QPen pen) {
    d->pen = pen;
}


/*!
Sets the item's shape to \a shape.
*/
void SerieFormat::setShape( const Global::Shape shape) {
    d->shape = shape;
}

/*!
Sets the serie's \c type.
*/
void SerieFormat::setType( const Global::Types type) {
    d->type = type;
}


/*!
Returns the item's shape to \a shape.
*/
Global::Shape SerieFormat::shape() const {
    return d->shape;
}

/*!
Sets the serie's \c type.
*/
Global::Types SerieFormat::type() const {
    return d->type;
}

}