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

namespace Thistle
{

SerieFormat::SerieFormat() : d( new SerieFormatPrivate() )
{
    d->brush = QBrush( Thistle::Colors::LightBlue );
    d->pen = QPen( QColor( Thistle::Colors::Blue ), 2 );
    d->shape = Thistle::Ellipse;
    d->type = Thistle::Line | Thistle::Dot;
}


QBrush SerieFormat::brush() const
{
    return d->brush;
}


QPen SerieFormat::pen() const
{
    return d->pen;
}


void SerieFormat::setBrush( const QBrush& brush)
{
    d->brush = brush;
}


void SerieFormat::setPen( const QPen& pen)
{
    d->pen = pen;
}


void SerieFormat::setShape( const Thistle::Shape shape)
{
    d->shape = shape;
}


void SerieFormat::setType( const Thistle::Types type)
{
    d->type = type;
}


Thistle::Shape SerieFormat::shape() const
{
    return d->shape;
}

Thistle::Types SerieFormat::type() const
{
    return d->type;
}

}