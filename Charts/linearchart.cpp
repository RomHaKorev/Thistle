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
  along with Marb.  If not, see <http://www.gnu.org/licenses/>.
 Marb  Copyright (C) 2013  Dimitry Ernot
*/
#include "linearchart.h"
#include "delegates.h"
#include <QItemDelegate>
#include <QPainter>
#include <QPaintEvent>
#include <QItemDelegate>
#include <QStyleOptionViewItem>


LinearChart::LinearChart( QWidget* parent ) : Chart( parent ) {
    myOrigin = QPointF(20, 10);
    myX = 0;
    myMinBottomMargin = 0;
    myPointDelegate = new PointDelegate( this );
    myBarDelegate = new BarDelegate( this );
    myVerticalXAxis = false;
}


QList<int> LinearChart::barStyleColumns() const {
    QList<int> bars;
    for ( int c = 0;  c < this->model()->columnCount(); ++c ) {
      if ( columnType( c ) == Marb::Bar ) {
        bars.append( c );
      }
    }
    return bars;
}


QList<int> LinearChart::calculateColumnsOrder() {
  QList<int> bars;
  QList<int> lines;
  QList<int> points;
  for ( int i = 0; i < this->model()->columnCount(); ++i ) {
    Marb::Types t = columnType( i );
    if ( t.testFlag( Marb::Bar ) ) {
      bars << i;
    } else if ( t.testFlag( Marb::Point ) ) {
      points << i;
    } else  {
      lines << i;
    }
  }
  bars << lines << points;
  return bars;
}


Marb::Types LinearChart::columnType( int column ) const {
    if ( myStyle.contains( column ) ) {
      return myStyle[ column ].type();
    }
    return ChartStyle().type();
}


QRectF LinearChart::itemRect( const QModelIndex& index ) const {
  QRect r;
  Marb::Types t = this->columnType( index.column() );
  QVariant var = index.data();
  if ( !var.isValid() ) {
    return QRect(0,0,1,1);
  }
  qreal value = var.toReal();
  qreal y = this->valueToPx(value);
  qreal x = myOrigin.x() + index.row() *  myX;
  qreal space = myX * 0.1;
  if ( t == Marb::Bar ) {
    QList<int> bars = this->barStyleColumns();
    qreal w = myX / bars.count();
    x += w * /*index.column()*/ bars.indexOf( index.column() );
    QPoint tl = QPoint( x + space/2, y ); // top left
    QPoint br = QPoint( x + w, myOrigin.y() ); // bottom right
    r = QRect( tl, br );
    if ( value < 0 ) {
      r.translate( 0, 1 );
    } else {
      r.translate( 0, -2 );
    }
  } else {
    r = QRect( -5, -5, 10 ,10 ).translated( x + myX/2, y );
  }
  return r.normalized();
}


void LinearChart::paintAxis( QPainter& painter) {
    painter.save();
    painter.setPen( QPen( QColor(Marb::LightGray), 1.5 ) );
    paintXAxis(painter);
    paintYAxis(painter);
    painter.restore();
}


void LinearChart::paintChart( QPainter& painter ) {
      painter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );
      this->paintAxis(painter);
      QList<int> orderedColumns = this->calculateColumnsOrder();
      foreach( int c, orderedColumns ) {
        this->paintValues( painter, c );
      }
      this->paintTextAxis(painter);
      painter.drawText( myTitleRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, myTitle );
      this->paintLegend( painter );
}


void LinearChart::paintColumnLegend(QPainter &painter, int column, QPoint pos, int maxHeight) {
    QPoint p1 = pos + QPoint( 10, - maxHeight/2 );
    QPoint p2 = pos + QPoint( 40, - maxHeight/2 );
    QPoint posText = pos + QPoint( 45, 0 );
    QString s(model()->headerData( column, Qt::Horizontal ).toString() );
    ChartStyle style = columnStyle( column );
    Marb::Types t = style.type();
    painter.drawText( posText, s );
    painter.save();
    painter.setPen( style.pen() );
    if ( t.testFlag( Marb::Line ) ) {
      painter.drawLine( p1, p2 );
    }
    if ( t.testFlag( Marb::Bar ) ) {
      int j = 0;
      foreach( int i, QList<int>() << 15 << 8 << 17 << 5 ) {
        painter.setPen( Qt::NoPen );
        painter.setBrush( style.brush() );
        QRect r( 10 + pos.x() + j * 8, pos.y() - i, 5, i );
        painter.drawRect( r );
        j += 1;
      }
    } else if ( t.testFlag( Marb::Point ) ) {
      QStyleOptionViewItem option;
      option.rect = QRect( p1.x() + abs(p1.x() - p2.x())/2 - 5, p1.y() - 5, 10, 10 );
      myPointDelegate->paint( &painter, option, this->model()->index( 0, column ) );
    }
    painter.restore();
}


void LinearChart::paintTextAxis( QPainter& painter) {
    painter.save();
    QFontMetrics metrics( font() );
    qreal h = metrics.height();
    QPoint textPos( h/2 , myOrigin.y() + 5 );
    painter.setPen( QPen( QColor(Marb::DarkGray), 1.5 ) );
    if ( myVerticalXAxis == true ) {
      qreal x = myX + myOrigin.x();
      int i = 0;
      while ( i < this->model()->rowCount() ) {
        QPoint p1( x, myOrigin.y() - 3  );
        QString s = this->model()->headerData( i, Qt::Vertical ).toString();
        painter.save();
        painter.rotate( -90 );
        painter.translate( -textPos.y() - metrics.width( s ), p1.x() - myX + h  + myX/2 );
        painter.drawText( 0, 0, s );
        painter.restore();
        ++i;
        x += myX;
      }
    } else {
      qreal x = myOrigin.x() + myX/2;
      int i = 0;
      int h = metrics.height();
      while ( i < this->model()->rowCount() ) {
        QString s = this->model()->headerData( i, Qt::Vertical ).toString();
        int w = metrics.width( s ) / 2;
        QPoint p1( x - w , myOrigin.y() + h );
        painter.drawText( p1, s );
        ++i;
        x += myX;
      }
    }
    qreal y = myMinBound;
    while ( y <= myMaxBound ) {
      QPoint p1( myOrigin.x(), valueToPx(y)  );
      if ( myOrder >= 1 ) {
        QString s = QString::number( qRound(y) );
        QRect r( QPoint( 0, p1.y() - h/2 ), QSize( myOrigin.x() - 5 ,h) );
        painter.drawText( r, Qt::AlignRight, s );
      }
      y += myTickSize;
    }
    painter.restore();
}


void LinearChart::paintValues( QPainter& painter, int column ) {
  Marb::Types t = this->columnType( column );
  QStyledItemDelegate* delegate = 0;
  if ( t.testFlag( Marb::Point ) ) {
    delegate = myPointDelegate;
  } else if ( t.testFlag( Marb::Bar ) ) {
    delegate = myBarDelegate;
  }
  int rows = this->model()->rowCount();
  painter.save();
  ChartStyle style = this->columnStyle( column );
  painter.setBrush( style.brush() );
  painter.setPen( style.pen() );

  bool isActive = true;
  if ( this->selectionModel() != 0 ) {
    QModelIndexList selectedIndexes = this->selectionModel()->selectedIndexes();
    if ( !selectedIndexes.isEmpty() ) {
      isActive = false;
      Q_FOREACH( QModelIndex idx, selectedIndexes ) {
        if ( idx.column() == column ) {
          isActive = true;
          break;
        }
      }
    }
  }

  for ( int r = 0; r < rows; ++r ) {
    QModelIndex index = this->model()->index( r, column );
    QStyleOptionViewItem option;
    qreal value = index.data().toReal();
    if ( value < 0 ) {
      option.decorationPosition = QStyleOptionViewItem::Bottom;
    } else {
      option.decorationPosition = QStyleOptionViewItem::Top;
    }
    if ( isActive == false ) {
      option.state = QStyle::State_Off;
    } else {
      option.state = QStyle::State_Selected;
    }
    option.rect = this->itemRect( index ).toRect();
    if ( ( (t | Marb::Line) == t) && ( r < (rows - 1) ) ) {
      if ( isActive == false ) {
        QPointF p1 = option.rect.center();
        QPointF p2 = this->itemRect( this->model()->index( r + 1, column ) ).center();
        QLineF line( p1, p2 );
        QLineF l = QLineF( line.pointAt( 0.5 ), line.p2() ).normalVector();
        l.setLength( 4 );
        QLinearGradient gradient( l.p1(), l.p2() );
        QColor c( Qt::darkGray );
        c.setAlpha( 50 );
        gradient.setColorAt( 0, c );
        gradient.setColorAt( 1, Qt::transparent );
        gradient.setSpread( QLinearGradient::ReflectSpread );
        painter.save();
        QPen pen( QBrush( gradient ), 8 );
        painter.setPen( pen );
        painter.drawLine( p1, p2 );
        painter.restore();
      } else {
        QPointF p1 = option.rect.center();
        QPointF p2 = this->itemRect( this->model()->index( r + 1, column ) ).center();
        painter.drawLine( p1, p2 );
      }
    }
    if ( delegate != 0 ) {
      delegate->paint( &painter, option, index );
    }
  }
  painter.restore();
}


void LinearChart::paintXAxis( QPainter& painter ) {
    QPoint p1( myOrigin.x(), myOrigin.y() );
    QPoint p2( myChartRect.topRight().x(), myOrigin.y() );
    painter.drawLine( p1, p2 );
    qreal x = myX + myOrigin.x();
    int i = 0;
    while (i < this->model()->rowCount() ) {
      QPoint p1( x, myOrigin.y() - 3  );
      QPoint p2 = p1 + QPoint( 0, 6 );
      painter.drawLine( p1, p2 );
      ++i;
      x += myX;
    }
}


void LinearChart::paintYAxis( QPainter& painter ) {
  QPoint p1( myOrigin.x(), myChartRect.y() );
    QPoint p2 = p1 + QPoint( 0, myChartRect.height() );
    painter.drawLine( p1, p2 );
    painter.save();
    QColor c = painter.pen().color();
    c.setAlpha( 150 );
    painter.setPen( QPen( c , 1 ) );
    qreal y = myMinBound;
    while ( y <= myMaxBound ) {
      QPoint p1( myOrigin.x(), valueToPx(y)  );
      QPoint p2 = p1 + QPoint( myValuesRect.width(), 0 );
      painter.drawLine( p1, p2 );
      y += myTickSize;
    }
    painter.restore();
}

void LinearChart::updateRects() {
  if ( this->model() == 0 ) {
        return;
  }
  this->defineRects();
  QFontMetrics metrics( this->font() );
  this->calculateBounds();
  myOrigin.setX( metrics.width( QString::number(-1 * myOrder) ) + metrics.width("0") * myNbDigits + myMarginX );
  myValuesRect = QRect( myChartRect );
  myValuesRect.setX( myOrigin.x() );
  myX = qreal( myValuesRect.width() ) / ( this->model()->rowCount() );
  this->setAlphaBeta();
  qreal delta = myValuesRect.bottom() - ( myOrigin.y() + myMinBottomMargin );
  if ( delta < 0 ) {
    myValuesRect.setHeight( myValuesRect.height() + delta );
    this->setAlphaBeta();
  }
  myTitleRect.moveTo( myChartRect.bottomLeft() );
  myTitleRect.translate( ( myChartRect.width() - myTitleRect.width())/2, 20 );
}

