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

#include "seriedelegates.h"
#include "../base/seriechart.h"
#include "../../Core/global.h"
#include "../base/serieformat.h"
#include <QPainter>
#include <QDebug>

namespace Thistle
{
AbstractChartDelegate::AbstractChartDelegate( AbstractChartDelegatePrivate* ptr, QWidget* parent ):
	QStyledItemDelegate(), d_ptr( ptr )
{}


void AbstractChartDelegate::setSerieFormatProxy( QPointer<SerieFormatProxy> proxy )
{
	d_ptr->proxy = proxy;
}


QPointer<SerieFormatProxy> AbstractChartDelegate::serieFormatProxy() const
{
	return d_ptr->proxy;
}


DotDelegate::DotDelegate( QPointer<SerieFormatProxy> proxy, QWidget* parent ):
	AbstractChartDelegate( new DotDelegatePrivate( proxy ), parent )
{}


BarDelegate::BarDelegate( QPointer<SerieFormatProxy> proxy, QWidget* parent ) :
AbstractChartDelegate( new BarDelegatePrivate( proxy ), parent )
{}


void AbstractChartDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	if ( option.state == QStyle::State_Off )
		d_ptr->paintDisabled(painter, option, index);
	else
		d_ptr->paintEnabled(painter, option, index);
}



}
