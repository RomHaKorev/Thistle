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

#ifndef DELEGATES_H
#define DELEGATES_H
#include <QStyledItemDelegate>

namespace Thistle
{

class AxisChart;


class AbstractChartDelegate : public QStyledItemDelegate
{
public:
    AbstractChartDelegate( QWidget* parent = 0 ) : QStyledItemDelegate( parent ) {}

    virtual void paintDisabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;

    virtual void paintEnabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;
};

class DotDelegate : public AbstractChartDelegate
{
    QPolygon createDiamond( const QRect& rect) const;

public:
    explicit DotDelegate( AxisChart* parent = 0 );
    virtual void paintDisabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paintEnabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class BarDelegate : public AbstractChartDelegate
{
public:
    explicit BarDelegate( AxisChart* parent = 0 );
    virtual void paintDisabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paintEnabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

}

#endif // DELEGATES_H
