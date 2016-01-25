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

#ifndef THISTLE_SERIEDELEGATES_H
#define THISTLE_SERIEDELEGATES_H
#include <QStyledItemDelegate>

#include "serieformatproxy.h"
#include "private/seriedelegates_p.h"

namespace Thistle
{

class AbstractChartDelegate : public QStyledItemDelegate
{
	Q_DECLARE_PRIVATE( AbstractChartDelegate )
protected:
	AbstractChartDelegatePrivate* d_ptr;
	AbstractChartDelegate( AbstractChartDelegatePrivate* ptr, QWidget* parent = 0 );
public:
	virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	void setSerieFormatProxy( QPointer<SerieFormatProxy> proxy );
	QPointer<SerieFormatProxy> serieFormatProxy() const;
};

class DotDelegate : public AbstractChartDelegate
{
	Q_DECLARE_PRIVATE( DotDelegate )
public:
	explicit DotDelegate( QPointer<SerieFormatProxy> proxy, QWidget* parent = 0 );
};

class BarDelegate : public AbstractChartDelegate
{
	Q_DECLARE_PRIVATE( BarDelegate )
public:
	explicit BarDelegate( QPointer<SerieFormatProxy> proxy, QWidget* parent = 0 );
};

}

#endif // THISTLE_SERIEDELEGATES_H
