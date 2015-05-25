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
#include "graphview.h"
#include <QScrollBar>

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <qmath.h>

#include "graphmodel.h"
#include "abstractgraphalgorithm.h"
#include "graphalgorithm.h"
#include "graphview_p.h"

#include "../kernel/itemdelegate.h"
static const qreal Pi = 3.14159265358979323846264338327950288419717;

namespace Thistle
{

GraphView::GraphView(QWidget *parent) : AbstractItemView( new GraphViewPrivate( this ), parent )
{
    Q_D( GraphView );
    setDragEnabled( true );
    ItemDelegate* delegate = new ItemDelegate( this );
    ItemStyle style = delegate->itemStyle();
    style.setShape( Thistle::Ellipse );
    delegate->setItemStyle( style );
    this->setItemDelegate( delegate );

    d->algorithm = new GraphAlgorithm( this );

    connect( d->algorithm, SIGNAL( updated() ), this, SLOT( update() ) );
    connect( d->algorithm, SIGNAL( updated() ), this->viewport(), SLOT( update() ) );

}

GraphView::~GraphView()
{
}


QPainterPath GraphView::itemPath( const QModelIndex& index ) const
{
    const Q_D( GraphView );
    QPainterPath path;
    //const Node& node = d->itemPos.value( index );
    Node node = d->algorithm->node( index );
    if ( node.isNull() )
        return path;

    QPointF pos = node.pos();

	QRect r( -20 + pos.x(), -20 + pos.y(), 40, 40 );
	path.addRect( r );
    path.translate( -this->horizontalOffset(), -this->verticalOffset() );

	

    return path;
}


GraphModel* GraphView::model() const
{
    const Q_D( GraphView );
    return d->model;
}

#if 0
void GraphView::mouseMoveEvent( QMouseEvent* event )
{
    Q_D( GraphView );
    QPoint p = QPoint( this->horizontalOffset(), this->verticalOffset() );
    if ( d->movableItem == true && d->movedItem.isValid() )
    {
        if ( !d->dragDropTime.isNull() )
        {
            QTime current = QTime::currentTime();
            if ( d->dragDropTime.msecsTo( current ) > 50 )
            {
                //d->itemPos[ d->movedItem ].setPos( event->pos() + p );

            }
        }
    }
    AbstractItemView::mouseMoveEvent( event );
}


void GraphView::mousePressEvent( QMouseEvent* event )
{
    Q_D( GraphView );
    if ( d->movableItem == true )
    {
        QModelIndex idx = this->indexAt( event->pos() );
        if ( idx.isValid() )
        {
            d->movedItem = idx;
            d->dragDropTime = QTime::currentTime();
        }
    }
    AbstractItemView::mousePressEvent( event );
}


void GraphView::mouseReleaseEvent( QMouseEvent* event )
{
    Q_D( GraphView );
    if ( d->movableItem == true )
    {
        if ( d->elasticItem == true )
        {
        }
        else
        {
            d->movedItem = QModelIndex();
        }
        d->dragDropTime = QTime();
    }
    AbstractItemView::mouseReleaseEvent( event );
}
#endif

void GraphView::paintEdges( QPainter& painter, const QPointF& offset ) const
{
    const Q_D( GraphView );
    Q_UNUSED( offset )
    painter.save();
    painter.setPen( QPen( QColor( Thistle::Colors::Gray ), 3 ) );
    painter.setBrush( QColor( Thistle::Colors::Gray ) );
    Q_FOREACH( Edge edge, d->model->edges() )
    {
		if ( !this->isSelected( edge.leftIndex ) && !this->isSelected( edge.rightIndex ) )
			painter.setOpacity( 0.5 );
		else
			painter.setOpacity( 1 );
        edge.paintEdge( painter, this->itemRect( edge.leftIndex ), this->itemRect( edge.rightIndex ) );
    }
    painter.restore();
}


void GraphView::paintEvent( QPaintEvent* /*event*/ )
{
    if ( this->model() == 0 )
    {
        return;
    }
    QPainter painter( viewport() );
    //painter.translate( -this->horizontalOffset(), -this->verticalOffset() );
    painter.setRenderHint( QPainter::Antialiasing );
    this->paintEdges( painter );
    this->paintItems( painter );
}


void GraphView::paintItems( QPainter& painter, const QPointF& offset ) const
{
	
	painter.save();
    for (int r = 0; r < this->model()->rowCount(); ++r )
    {
        QModelIndex idx = this->model()->index( r, 0 );
		QStyleOptionViewItem option;
		option.rect = this->itemRect( idx ).toRect();
		option.showDecorationSelected = this->isSelected( idx );
		this->itemDelegate()->paint( &painter, option, idx );
    }
	painter.restore();
}

void GraphView::setScrollBarValues()
{
    Q_D( GraphView );

	QSizeF realSize = d->boundingRect.size() + ( this->itemRect( this->model()->index( 0, 0 ) ).size() );
	this->horizontalScrollBar()->setMinimum( 0 );
	this->verticalScrollBar()->setMinimum( 0 );
	if ( (realSize.width() - this->width() + 50) > 0 )
		this->horizontalScrollBar()->setMaximum( realSize.width() - this->width() + 50 );
	else
		this->horizontalScrollBar()->setMaximum( 0 );

	if ( (realSize.height() - this->height() + 50) > 0 )
		this->verticalScrollBar()->setMaximum( realSize.height() - this->height() + 50 );
	else
		this->verticalScrollBar()->setMaximum( 0 );
}


void GraphView::setModel( GraphModel* model )
{
    Q_D( GraphView );
    d->model = model;
    AbstractItemView::setModel( model );
    connect( model, SIGNAL(updateEdges()), this, SLOT(updateValues()));
}


void GraphView::updateValues()
{
    Q_D( GraphView );
    d->algorithm->run();
}


AbstractGraphAlgorithm* GraphView::algorithm() const
{
    const Q_D( GraphView );
    return d->algorithm;
}

void GraphView::setAlgorithm( AbstractGraphAlgorithm* algorithm )
{
    Q_D ( GraphView );
    d->algorithm = algorithm;
    connect( d->algorithm, SIGNAL( updated() ), this, SLOT( update() ) );
    connect( d->algorithm, SIGNAL( updated() ), this->viewport(), SLOT( update() ) );
}

}
