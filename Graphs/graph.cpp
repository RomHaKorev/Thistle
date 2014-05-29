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
#include "graph.h"
#include <QScrollBar>

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <qmath.h>

#include "graphmodel.h"
#include "graphalgorithm.h"
#include "graph_p.h"

#include "../kernel/itemdelegate.h"
static const qreal Pi = 3.14159265358979323846264338327950288419717;

namespace Thistle {

    /*!
    Constructs a \a Graph view with the given \a parent.
    */
    Graph::Graph(QWidget *parent) : AbstractItemView( new GraphPrivate(), parent ) {
        Q_D( Graph );
        setDragEnabled( true );
        ItemDelegate* delegate = new ItemDelegate( this );
        ItemStyle style = delegate->itemStyle();
        style.setShape( Thistle::Ellipse );
        delegate->setItemStyle( style );
        this->setItemDelegate( delegate );

        d->algorithm = new GraphAlgorithm( this );

        connect( d->algorithm, SIGNAL( update() ), this, SLOT( update() ) );
        connect( d->algorithm, SIGNAL( update() ), this->viewport(), SLOT( update() ) );

    }

    Graph::~Graph() {
    }



    QModelIndex Graph::indexAt( const QPoint& point ) const {
        const Q_D( Graph );
        QPoint p = point    + QPoint( horizontalOffset(), verticalOffset() );

        if ( this->model() == 0 ) return QModelIndex();

        Q_FOREACH( QModelIndex index, d->itemPos.keys() ) {
            QPainterPath path = this->itemPath( index );
            if ( path.contains( p ) ) {
                return index;
            }
        }
        return QModelIndex();
    }


    QPainterPath Graph::itemPath( const QModelIndex& index ) const {
        const Q_D( Graph );
        QPainterPath path;
        //const Node& node = d->itemPos.value( index );
        Node node = d->algorithm->node( index );
        if ( node.isNull() ) {
            return path;
        }
        //QPointF pos = d->itemPos.value( index ).pos() - QPointF( horizontalOffset(), verticalOffset() );
        QPointF pos = node.pos();
        path.addRect( QRect(-20,-20,40,40).translated( pos.x(), pos.y() ) );
        return path;
    }


    GraphModel* Graph::model() const {
        const Q_D( Graph );
        return d->model;
    }


    void Graph::mouseMoveEvent( QMouseEvent* event ) {
        Q_D( Graph );
        QPoint p = QPoint( this->horizontalOffset(), this->verticalOffset() );
        if ( d->movableItem == true && d->movedItem.isValid() ) {
            if ( !d->dragDropTime.isNull() ) {
                QTime current = QTime::currentTime();
                if ( d->dragDropTime.msecsTo( current ) > 50 ) {
                    //d->itemPos[ d->movedItem ].setPos( event->pos() + p );

                }
            }
        }
        AbstractItemView::mouseMoveEvent( event );
    }


    void Graph::mousePressEvent( QMouseEvent* event ) {
        Q_D( Graph );
        if ( d->movableItem == true ) {
            QModelIndex idx = this->indexAt( event->pos() );
            if ( idx.isValid() ) {
                d->movedItem = idx;
                d->dragDropTime = QTime::currentTime();
            }
        }
        //AbstractItemView::mousePressEvent( event );
    }


    void Graph::mouseReleaseEvent( QMouseEvent* event ) {
        Q_D( Graph );
        //AbstractItemView::mouseReleaseEvent( event );
        if ( d->movableItem == true ) {
            if ( d->elasticItem == true ) {
            } else {
                d->movedItem = QModelIndex();
            }
            d->dragDropTime = QTime();
        }
    }


    void Graph::paintEdges( QPainter& painter, const QPointF& offset ) const {
        const Q_D( Graph );
        Q_UNUSED( offset )
            painter.save();
        painter.setPen( QPen( QColor( Thistle::Colors::Gray ), 3 ) );
        painter.setBrush( QColor( Thistle::Colors::Gray ) );
        Q_FOREACH( Edge edge, d->model->edges() ) {
            edge.paintEdge( painter, this->itemRect( edge.leftIndex ), this->itemRect( edge.leftIndex ) );
        }
        painter.restore();
    }


    void Graph::paintEvent( QPaintEvent* /*event*/ ) {
        Q_D( Graph );
        if ( this->model() == 0 ) {
            return;
        }
        //updateValues();
        QPainter painter(viewport());
        painter.setRenderHint( QPainter::Antialiasing );
        this->paintEdges( painter );
        this->paintItems( painter );
    }


    void Graph::paintItems( QPainter& painter, const QPointF& offset ) const {
        const Q_D( Graph );
        //Q_FOREACH( QModelIndex idx, d->itemPos.keys()    ) {
        for (int r = 0; r < this->model()->rowCount(); ++r ) {
            QModelIndex idx = this->model()->index( r, 0 );
            QStyleOptionViewItem option;
            option.rect = this->itemRect( idx ).translated( offset.x(), offset.y() ).toRect();
            this->itemDelegate()->paint( &painter, option, idx );
        }
        /*for (int r = 0; r < this->model()->rowCount(); ++r ) {
        QModelIndex idx = this->model()->index( r, 0 );
        QStyleOptionViewItem option;
        option.rect = this->itemRect( idx ).translated( offset.x(), offset.y() ).toRect();
        qDebug() << Q_FUNC_INFO << option.rect;
        this->itemDelegate()->paint( &painter, option, idx );
        }*/
    }

    void Graph::setScrollBarValues() {
        Q_D( Graph );
        qreal dw = qMax( 0.0, d->realSize.width() - width()	);
        qreal dh = qMax ( 0.0, d->realSize.height() - height() );
        horizontalScrollBar()->setRange( 0, dw );
        verticalScrollBar()->setRange( 0, dh );
        d->itemOffset = QPoint( 0, 0 );
    }


    void Graph::setModel( GraphModel* model ) {
        Q_D( Graph );
        d->model = model;
        AbstractItemView::setModel( model );
        connect( model, SIGNAL(updateEdges()), this, SLOT(updateValues()));AbstractItemView::setModel(model);
    }


    void Graph::updateValues() {
#if 0
        Q_D( Graph );
        if ( d->model == 0 ) {
            return;
        }
        d->itemPos.clear();
        int rows = d->model->rowCount();
        int nbRows = qFloor( qSqrt( rows ) );
        int x = 10;
        int y = 10;
        int i = 0;
        for( int r = 0; r < rows; ++ r ) {
            QModelIndex idx = d->model->index( r, 0 );
            if ( idx.isValid() && idx.data().isValid() ) {
                Node n;
                n.setPos( QPointF( x, y ) );
                //n.setPos(10 + qrand() % 200, 10 + qrand() % 200);
                d->itemPos.insert( idx, n );
                x += 100;
                ++i;
                if ( i > nbRows ) {
                    x = 10;
                    y += 100;
                    i = 0;
                }
            }
        }
        Q_FOREACH ( Edge e, d->model->edges() ) {
            d->itemPos[ e.leftIndex ].addEdge( &(d->itemPos[ e.rightIndex ]) );
            d->itemPos[ e.rightIndex ].addEdge( &(d->itemPos[ e.leftIndex ]) );
        }

        d->timer->start();
        d->oldLength = 0;
#endif
        Q_D( Graph );
        d->algorithm->run();
    }

}