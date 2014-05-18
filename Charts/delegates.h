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

namespace Thistle {

    class AxisChart;

    class AbstractChartDelegate : public QStyledItemDelegate {
    public:
        AbstractChartDelegate( QWidget* parent = 0 ) : QStyledItemDelegate( parent ){}

        /*!
        Paints the delegate's disabled render.

        An item is considered as disabled when the selectionModel doesn't contain any index in the column of \a index.

        For example, by default, the \a DotDelegate draw a disabled item as a blurred gray point.
        */
        virtual void paintDisabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;

        /*!
        Paints the delegate's enabled render.

        An item is considered as enabled when the selectionModel is empty or contains an index in the column of \a index.
        */
        virtual void paintEnabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;

        /*!
        Paints the data of the \a index relative to the shape set in the \a SerieFormat corresponding to the \a index column.
        */
        virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;
    };

    class DotDelegate : public AbstractChartDelegate {
        QPolygon createDiamond( const QRect& rect) const;

    public:
        explicit DotDelegate( AxisChart* parent = 0 );
        virtual void paintDisabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual void paintEnabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    };

    class BarDelegate : public AbstractChartDelegate {
    public:
        explicit BarDelegate( AxisChart* parent = 0 );
        virtual void paintDisabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual void paintEnabled(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    };

}

#endif // DELEGATES_H
