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
Thistle    Copyright (C) 2013    Dimitry Ernot & Romha Korev
*/

#include "treeview.h"
#include "../kernel/itemdelegate.h"
#include <QScrollBar>
#include <QPainter>
#include <QPaintEvent>
#include <QStandardItem>

#include <QAbstractScrollArea>

#include "../kernel/global.h"

namespace Thistle
{

TreeView::TreeView(QWidget* parent) : AbstractItemView( new TreeViewPrivate(), parent )
{
    Q_D( TreeView );
    this->setItemDelegate( d->delegate );
    d->layout = new TreeLayout( this );
    d->layout->setItemSize( QSize( 30, 20 ) );
    d->layout->setSpacing( QMargins( 10, 20, 10, 20 ) );

    this->horizontalScrollBar()->setMinimum( 0 );
    this->verticalScrollBar()->setMinimum( 0 );
}

TreeView::TreeView( TreeViewPrivate* d, QWidget* parent ) : AbstractItemView( d, parent )
{
    this->setItemDelegate( d->delegate );
    d->layout = new TreeLayout( this );
}

TreeView::~TreeView()
{
}


QPen TreeView::connectionPen() const
{
    const Q_D( TreeView );
    return d->connectionPen;
}


QModelIndex TreeView::indexAt(const QPoint& point) const
{
	int dx = this->horizontalScrollBar()->value();
	int dy = this->verticalScrollBar()->value();
    QPoint p = point + QPoint( dx, dy );

    return this->layout()->indexAt( p );
}


QPainterPath TreeView::itemPath( const QModelIndex& index ) const
{
    QPainterPath path;
    path.addRect( this->layout()->itemRect( index ) );
	path.translate( -this->horizontalOffset(), -this->verticalOffset() );
    return path;
}


/*QRectF TreeView::itemRect( const QModelIndex& index ) const
{
    QPointF p = QPoint( this->horizontalOffset(), this->verticalOffset() );
    return this->layout()->itemRect( index ).translated( -p.toPoint() );
}*/


void TreeView::paintEvent( QPaintEvent* event)
{
    if ( this->layout() == 0 )
        return;

    QPainter painter( viewport() );
    painter.setClipRect( event->rect() );
    painter.setRenderHint( QPainter::Antialiasing );
    this->paintConnections( painter, QPointF(0,0) );
    this->paintItems( painter, QPointF(0,0) );

    //painter.drawRect( this->layout()->boundingRect() );
}


void TreeView::paintItems( QPainter& painter, const QPointF& offset ) const
{
  Q_UNUSED( offset )
    Q_FOREACH ( QModelIndex index, this->layout()->validIndexes() )
    {
        QStyleOptionViewItem option;
        option.rect = this->itemRect( index ).toRect();
		option.showDecorationSelected = this->isSelected( index );
        this->itemDelegate()->paint( &painter, option, index );
    }
}

void TreeView::paintConnections( QPainter& painter, const QPointF& offset ) const
{
    const Q_D( TreeView );
    painter.save();
    painter.setPen( d->connectionPen );
    Q_FOREACH ( QModelIndex index, this->layout()->validIndexes() )
    {
        this->paintConnectionsFor( painter, index, offset );
    }
    painter.restore();
}


void TreeView::paintConnectionsFor( QPainter& painter, const QModelIndex& index, const QPointF& offset ) const
{
    painter.save();
    QModelIndex parent = this->model()->parent( index );
    if ( parent.isValid() )
    {
        QPointF p1 = this->itemRect( index ).translated( offset.x(), offset.y() ).center();
        QPointF p2 = this->itemRect( parent ).translated( offset.x(), offset.y() ).center();
        painter.drawLine( p1, p2 );
    }
    painter.restore();
}

void TreeView::resizeEvent( QResizeEvent* event )
{
    QAbstractItemView::resizeEvent( event );
    this->setScrollBarValues();
}


void TreeView::setConnectionPen( const QPen& pen )
{
    Q_D( TreeView );
    d->connectionPen = pen;
}

void TreeView::setScrollBarValues()
{
    if ( this->layout() == 0 )
    {
        this->horizontalScrollBar()->setMinimum( 0 );
        this->verticalScrollBar()->setMinimum( 0 );
        this->horizontalScrollBar()->setMaximum( 0 );
        this->verticalScrollBar()->setMaximum( 0 );
    }

    QPointF centerPoint = this->itemRect( this->model()->index( 0, 0 ) ).center();

    QSize size = this->viewport()->size();
    QRectF viewportRect( centerPoint - QPointF( size.width()/2, size.height()/2 ),
                         size );

    QRectF neededRect = this->layout()->boundingRect();

    int w = neededRect.width() - this->viewport()->width();
    int h = neededRect.height() - this->viewport()->height();

    if ( w > 0 )
    {

        this->horizontalScrollBar()->setMaximum(  w );
    }
    else
    {
        this->horizontalScrollBar()->setMaximum( 0 );
    }

    if ( h > 0 )
    {
        this->verticalScrollBar()->setMaximum(  h );
    }
    else
    {
        this->verticalScrollBar()->setMaximum( 0 );
    }

}


void TreeView::updateValues()
{
    if ( this->layout() == 0 )
        return;

    this->layout()->update();
    this->setScrollBarValues();
}


TreeLayout* TreeView::layout() const
{
    const Q_D( TreeView );
    return d->layout;
}

void TreeView::setLayout( TreeLayout* layout )
{
    Q_D( TreeView );
    d->layout = layout;
    layout->setView( this );
}


void TreeView::setSelection( const QRect& rect, QItemSelectionModel::SelectionFlags command )
{
	QPainterPath contentsPath;
	contentsPath.addRect( rect );
	int count = 0;
	Q_FOREACH( QModelIndex index, this->layout()->validIndexes() )
	{
		QPainterPath path = this->itemPath( index );
		if ( !path.intersected( contentsPath ).isEmpty() )
		{
			count += 1;
			this->selectionModel()->select( index, command );
		}
	}

	if ( count == 0 )
		this->selectionModel()->clear();

	this->viewport()->update();
}

}
