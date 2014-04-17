/*
 This file is part of Marb.

    Marb is free software: you can redistribute it and/or modify
    it under the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    Marb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with Marb.    If not, see <http://www.gnu.org/licenses/>.

 Marb    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/

#ifndef DELEGATES_H
#define DELEGATES_H
#include <QStyledItemDelegate>

namespace Marb {

class AxisChart;

class AbstractChartDelegate : public QStyledItemDelegate {
public:
    AbstractChartDelegate( QWidget* parent = 0 ) : QStyledItemDelegate( parent ){}
    virtual void paintDisabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;
    virtual void paintEnabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;
};

class PointDelegate : public AbstractChartDelegate {
    QPolygon createDiamond( const QRect& rect) const;

public:
    explicit PointDelegate( AxisChart* parent = 0 );
    virtual void paintDisabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paintEnabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class BarDelegate : public AbstractChartDelegate {
public:
    explicit BarDelegate( AxisChart* parent = 0 );
    virtual void paintDisabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paintEnabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

}

#endif // DELEGATES_H
