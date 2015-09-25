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

#ifndef THISTLE_PIECHART3D_H
#define THISTLE_PIECHART3D_H

#include "piechart.h"
#include "private/piechart_p.h"

namespace Thistle
{

class PieChart3DPrivate;

class PieChart3D : public PieChart
{
	Q_OBJECT
	Q_DECLARE_PRIVATE( PieChart );
public:
	PieChart3DPrivate* test;

	enum Render
	{
		Plain = 0,
		WireFrame = 1,
		Translucent = 2
	};

	Q_ENUMS( Render );

	explicit PieChart3D(QWidget *parent = 0);
	void setRender( PieChart3D::Render );
	virtual void paint( QPainter& painter );
protected:
	virtual QPainterPath itemSidesPath( const QModelIndex& index ) const;
	virtual QPainterPath itemExternalPart( qreal angle, qreal delta, bool splitted = false ) const;
	virtual QPainterPath side( qreal angle, QPointF centerOffset, bool splitted ) const;
	virtual void configureColor(QPainter &painter, QColor base, int flag) const;

	virtual void paintSides( QPainter& painter );
	virtual void paintExternal( QPainter& painter, bool top );
	virtual void paintLeft( QPainter& painter, QColor color, QPointF offset = QPointF(0,0) );
};

}

#endif // THISTLE_PIECHART3D_H
