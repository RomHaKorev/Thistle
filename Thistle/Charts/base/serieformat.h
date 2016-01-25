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

#ifndef THISTLE_SERIEFORMAT_H
#define THISTLE_SERIEFORMAT_H

#include <QtGui/QBrush>
#include <QtGui/QPen>
#include "../../Core/global.h"

#include "private/serieformat_p.h"

class SerieFormatDigester;

namespace Thistle
{

class SerieFormat
{
	friend class SerieFormatDigester;
protected:
	SerieFormatPrivate* d;
public:
	SerieFormat();
	void reset();

	void setBrush( const QBrush& brush);
	void setPen( const QPen& pen);
	void setShape( const Thistle::Shape shape);
	void setType( const Thistle::Types type);
	QBrush brush() const;
	QPen pen() const;
	Thistle::Shape shape() const;
	Thistle::Types type() const;
};

}

#endif // THISTLE_SERIEFORMAT_H
