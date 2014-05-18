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

#ifndef SERIEFORMAT_H
#define SERIEFORMAT_H

#include <QBrush>
#include <QPen>
#include "../kernel/global.h"

//#include "linearchart.h"

namespace Thistle {

    struct SerieFormatPrivate {
        QBrush brush;
        QPen pen;
        Global::Shape shape;
        Global::Types type;
    };


    class SerieFormat {
    protected:
        SerieFormatPrivate* d;
    public:
        SerieFormat();
        void setBrush( const QBrush brush);
        void setPen( const QPen pen);
        void setShape( const Global::Shape shape);
        void setType( const Global::Types type);
        QBrush brush() const;
        QPen pen() const;
        Global::Shape shape() const;
        Global::Types type() const;
    };

}

#endif // CHARTSTYLE_H
