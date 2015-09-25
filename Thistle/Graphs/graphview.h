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

#ifndef THISTLE_GRAPH_H
#define THISTLE_GRAPH_H

#include <QTimer>
#include <QTime>
#include "../kernel/abstractitemview.h"
#include "node.h"
#include "edge.h"

#include "private/graphview_p.h"

class GraphModel;

namespace Thistle
{

class GraphView : public AbstractItemView
{
	friend class GraphAlgorithm;
	Q_OBJECT
	Q_DECLARE_PRIVATE( GraphView );

protected:

	//virtual QModelIndex indexAt( const QPoint& point ) const;
	virtual void setScrollBarValues();
public:
	explicit GraphView( QWidget* parent = 0 );
	virtual ~GraphView();

	virtual QPainterPath itemPath(const QModelIndex &index) const;

	void paintEvent( QPaintEvent* event );
	void paintEdges( QPainter& painter, const QPointF& offset = QPointF( 0, 0 ) ) const;
	void paintItems( QPainter& painter, const QPointF& offset = QPointF( 0, 0 ) ) const;

	void setModel( GraphModel* model );
	GraphModel* model() const;

	AbstractGraphAlgorithm* algorithm() const;
	void setAlgorithm( AbstractGraphAlgorithm* algorithm );

	void setNodeBoundingRect( const QRect& bounding )
	{
		Q_D( GraphView );
		d->boundingRect = bounding;
	}

public slots:
	void updateValues();

};

}

#endif // THISTLE_GRAPH_H
