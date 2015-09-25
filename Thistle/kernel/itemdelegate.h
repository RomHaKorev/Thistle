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

#ifndef THISTLE_ITEMDELEGATE_H
#define THISTLE_ITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPen>
#include <QBrush>
#include "global.h"

#include "itemstyle.h"

namespace Thistle
{

class ItemDelegatePrivate;

class ItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

private:
	QPolygon createDiamond( const QRect& r ) const;
protected:
	ItemDelegatePrivate* d_ptr;

public:
	explicit ItemDelegate( QWidget* parent = 0 );
	virtual ~ItemDelegate();

	void setItemStyle( const ItemStyle& myStyle );
	ItemStyle& itemStyle() const;
	QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
	void setEditorData( QWidget *editor, const QModelIndex &index ) const;
	void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

}
#endif // THISTLE_ITEMDELEGATE_H
