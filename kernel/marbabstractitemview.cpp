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
#include "marbabstractitemview.h"
#include "marbitemdelegate.h"
#include "Marb.h"
#include <QScrollBar>
#include <QPainter>
#include <QPaintEvent>
#include <QStandardItem>

#include <QDebug>


#include "marbabstractitemview_p.h"

MarbAbstractItemView::MarbAbstractItemView(QWidget *parent) : QAbstractItemView( parent ) {
    setItemDelegate( d->delegate );
}


QList<int> MarbAbstractItemView::calculateColumnsOrder() const {
    QList<int> l;
    for ( int i = 0; i < this->model()->columnCount(); ++i ) {
        l << i;
    }
    return ( l );
}


QMargins MarbAbstractItemView::contentsMargins() const { 
    return d->margins;
}

QRect MarbAbstractItemView::contentsRect() const {
    return QRect( d->margins.left(), d->margins.top(), width() - d->margins.left() - d->margins.right(),
                                               height() - d->margins.top() - d->margins.bottom() );
}


void MarbAbstractItemView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
    QAbstractItemView::dataChanged(topLeft, bottomRight);
    update( this->model()->index(0, 0) );
}


int MarbAbstractItemView::horizontalOffset() const {
    return horizontalScrollBar()->value();
}


QModelIndex MarbAbstractItemView::indexAt(const QPoint &point) const {
    QPoint p = point    + QPoint( horizontalOffset(), verticalOffset() );
    if ( this->model() == 0 ) {
        return QModelIndex();
    }

    Q_FOREACH( QModelIndex index, d->itemPos.keys() ) {
            qDebug() << index.data().toString();
            QPainterPath path = this->itemPath( index );
            if ( path.contains( p ) ) {
                return index;
            }
    }
    return QModelIndex();
}


bool MarbAbstractItemView::isIndexHidden(const QModelIndex& /*index*/ ) const {
    return false;
}


QRectF MarbAbstractItemView::itemRect( const QModelIndex& index ) const {
    return this->itemPath( index ).boundingRect();
}


QModelIndex MarbAbstractItemView::moveCursor( QAbstractItemView::CursorAction cursorAction,
                                             Qt::KeyboardModifiers /*modifiers*/ ) {
    switch( cursorAction ) {
    case QAbstractItemView::MoveDown:
    break;
    default:
    break;
    }
    return QModelIndex();
}


void MarbAbstractItemView::resizeEvent( QResizeEvent* event ) {
    setScrollBarValues();
    QAbstractItemView::resizeEvent( event );
}


int MarbAbstractItemView::rows(const QModelIndex &index) const {
    return this->model()->rowCount(model()->parent(index));
}


void MarbAbstractItemView::rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end) {
    QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
    updateValues();
    viewport()->update();
    setScrollBarValues();
}


void MarbAbstractItemView::rowsInserted(const QModelIndex& /*parent*/, int /*start*/, int /*end*/) {
    updateValues();
    viewport()->update();
    setScrollBarValues();
}


void MarbAbstractItemView::scrollTo(const QModelIndex& index, ScrollHint hint ) {
    Q_UNUSED( hint )
    QRect area = viewport()->rect();
    QRect rect = visualRect(index);
    horizontalScrollBar()->setValue( area.width() / 2 );
    return;
    if (rect.left() < area.left())
            horizontalScrollBar()->setValue(
                    horizontalScrollBar()->value() + rect.left() - area.left());
    else if (rect.right() > area.right())
            horizontalScrollBar()->setValue(
                    horizontalScrollBar()->value() + qMin(
                            rect.right() - area.right(), rect.left() - area.left()));
    if (rect.top() < area.top())
            verticalScrollBar()->setValue(
                    verticalScrollBar()->value() + rect.top() - area.top());
    else if (rect.bottom() > area.bottom())
            verticalScrollBar()->setValue(
                    verticalScrollBar()->value() + qMin(
                            rect.bottom() - area.bottom(), rect.top() - area.top()));
    update();
}


void MarbAbstractItemView::setModel(QAbstractItemModel *model) {
    QAbstractItemView::setModel( model );
    updateValues();
}


void MarbAbstractItemView::setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command ) {
    QRect contentsRect = rect.translated(
                    this->horizontalScrollBar()->value(),
                    this->verticalScrollBar()->value()).normalized();
    int rows = this->model()->rowCount( this->rootIndex() );
    QList<int> columns = this->calculateColumnsOrder();
    int count = 0;
    QPainterPath contentsPath;
    contentsPath.addRect( contentsRect );
    for ( int row = 0; row < rows; ++row ) {
        Q_FOREACH( int col, columns ) {
            QModelIndex index = this->model()->index( row, col, this->rootIndex() );
            QPainterPath path = this->itemPath( index );
            if ( !path.intersected(contentsPath).isEmpty() ) {
                count += 1;
                this->selectionModel()->select( index, command );
            }
        }
    }
    if ( count == 0 ) {
        this->selectionModel()->clear();
    }
    this->viewport()->update();
}


int MarbAbstractItemView::verticalOffset() const {
    return verticalScrollBar()->value();
}


QRect MarbAbstractItemView::visualRect(const QModelIndex &index) const {
    QRect rect = itemRect(index).toRect();
    return rect;
}


QRegion MarbAbstractItemView::visualRegionForSelection(const QItemSelection &selection) const {
    return QRegion( QRect( 0, 0, this->width(), this->height() ) );
}