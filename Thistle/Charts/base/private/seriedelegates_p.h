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
along with Thistle. If not, see <http://www.gnu.org/licenses/>.

Thistle	Copyright (C) 2013	Dimitry Ernot & Romha Korev
*/

#ifndef THISTLE_SERIEDELEGATES_P_H
#define THISTLE_SERIEDELEGATES_P_H
#include <QStyleOptionViewItem>
#include <QPointer>

#include "../serieformatproxy.h"

namespace Thistle
{

class AbstractChartDelegatePrivate
{
public:
	QPointer<SerieFormatProxy> proxy;
	AbstractChartDelegatePrivate( QPointer<SerieFormatProxy> serieFormatProxy ): proxy( serieFormatProxy ) {}
	virtual void paintDisabled( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const = 0;
	virtual void paintEnabled( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const = 0;
};

class DotDelegatePrivate: public AbstractChartDelegatePrivate
{
	QPolygon createDiamond( const QRect& rect) const;
public:
	DotDelegatePrivate( QPointer<SerieFormatProxy> proxy );
	virtual void paintDisabled( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	virtual void paintEnabled( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
};

class BarDelegatePrivate: public AbstractChartDelegatePrivate
{
public:
	BarDelegatePrivate( QPointer<SerieFormatProxy> proxy );
	virtual void paintDisabled( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	virtual void paintEnabled( QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};

}

#endif // THISTLE_SERIEDELEGATES_P_H
