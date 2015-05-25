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

#include "seriechart.h"

#include <QFontMetrics>
#include <qmath.h>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QMargins>

#include <QDebug>

#include "../../kernel/abstractitemview_p.h"
#include "private/seriechart_p.h"
#include "../../kernel/global.h"
#include "abstractcoordinatesystemview.h"
#include "seriedelegates.h"

namespace Thistle
{

SerieChart::SerieChart( AbstractCoordinateSystemView* coordSysView, QWidget* parent ) : AbstractChart( new SerieChartPrivate( this, coordSysView ), parent )
{}

SerieChart::SerieChart( SerieChartPrivate* d, QWidget* parent ) : AbstractChart( d, parent )
{
  //this->setCoordSysView( d->coordinateSys );
  d->pointDelegate->setParent( this );
  d->barDelegate->setParent( this );
}

SerieChart::~SerieChart()
{}


void SerieChart::scan()
{
    Q_D( SerieChart );
    d->readModel();
}


void SerieChart::updateRects()
{
  Q_D( SerieChart );

  if ( this->model() == 0 )
  {
    return;
  }

  this->scan();
  
  d->coordinateSystemView()->setRect( this->contentsRect() );
  d->coordinateSystemView()->update();
}


void SerieChart::setCoordSysView( AbstractCoordinateSystemView* axis )
{
    Q_D( SerieChart );
  d->setCoordinateSystemView( axis );
}


void SerieChart::setModel( QAbstractItemModel* model )
{
    Q_D( SerieChart );
    QAbstractItemView::setModel( model );
    d->coordinateSystemView()->setModel( model );
    this->process();
}

void SerieChart::paintSerie( QPainter& painter, int column )
{
  Q_D( SerieChart );
  Thistle::Types t = this->columnType( column );

  d->selectDelegate( t );

  painter.save();
  SerieFormat style = this->serieFormat( column );
  painter.setBrush( style.brush() );
  painter.setPen( style.pen() );

  bool isActive = this->isActiveColumn( column );

  d->paint( painter, column, t, isActive );

  painter.restore();
}


QList<int> SerieChart::barStyleColumns() const
{
  QList<int> bars;
  for ( int c = 0; c < this->model()->columnCount(); ++c )
  {
    if ( columnType( c ) == Thistle::Bar )
    {
      bars.append( c );
    }
  }
  return bars;
}


QList<int> SerieChart::calculateColumnsOrder() const
{
  QList<int> areas;
  QList<int> bars;
  QList<int> lines;
  QList<int> points;

  QList<int> areasDisabled;
  QList<int> barsDisabled;
  QList<int> linesDisabled;
  QList<int> pointsDisabled;

  for ( int i = 0; i < this->model()->columnCount(); ++i )
  {
    Thistle::Types t = columnType( i );
    if ( this->isActiveColumn( i ) )
    {
      if ( t.testFlag( Thistle::Area ) )
        areas << i;
      else if ( t.testFlag( Thistle::Bar ) )
        bars << i;
      else if ( t.testFlag( Thistle::Dot ) )
        points << i;
      else
        lines << i;
    }
    else
    {
      if ( t.testFlag( Thistle::Area ) )
        areasDisabled << i;
      else if ( t.testFlag( Thistle::Bar ) )
        barsDisabled << i;
      else if ( t.testFlag( Thistle::Dot ) )
        pointsDisabled << i;
      else
        linesDisabled << i;
    }
  }
  areasDisabled << barsDisabled << linesDisabled << pointsDisabled << areas << bars << lines << points;
  return areasDisabled;
}

Thistle::Types SerieChart::columnType( int column ) const
{
  const Q_D( SerieChart );
  if ( d->style.contains( column ) )
  {
    return d->style[ column ].type();
  }
  return SerieFormat().type();
}

bool SerieChart::isActiveColumn( int column ) const
{
  if ( this->selectionModel() != 0 )
  {
    QModelIndexList selectedIndexes = this->selectionModel()->selectedIndexes();
    if ( !selectedIndexes.isEmpty() )
    {
      Q_FOREACH( QModelIndex idx, selectedIndexes )
      {
        if ( idx.column() == column )
        {
          return true;
        }
      }
      return false;
    }
  }
  return true;
}


void SerieChart::paintEvent( QPaintEvent* ev )
{
  Q_UNUSED( ev )

  Q_D( SerieChart );

  QPainter painter( this->viewport() );
  painter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );

  d->coordinateSystemView()->paintBack( painter );

  QList<int> ordered = this->calculateColumnsOrder();
  Q_FOREACH( int c, ordered )
  {
    this->paintSerie( painter, c );
  }

  d->coordinateSystemView()->paintFront( painter );
}

}
