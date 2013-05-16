#include "linearchart.h"
#include "delegates.h"
#include <QDebug>
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

void LinearChart::processSpec() {
    myChartRect = QRect( QPoint(10, 20),  viewport()->size() - QSize( 20, 40 ) );
    QFontMetrics metrics( font() );

    calculateLegendRect();
    myChartRect.setHeight( myChartRect.height() - myLegendRect.height() - 10 );
    myChartRect.translate( 0, myLegendRect.height() + 10 );

    if ( myTitle != "" ) {
      QRect r( 0, 0, myChartRect.width() - 40, 0 );
      myTitleRect = metrics.boundingRect( r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, myTitle );

      myChartRect.setHeight( myChartRect.height() - myTitleRect.height() );
      myTitleRect.translate( myOrigin.x(), myChartRect.bottom() );
    }

    calculateBounds();
    int w = metrics.width( QString::number(-1 * myOrder) );
    myOrigin.setX( myMarginX + w );

    myValuesRect = QRect( myChartRect );
    myValuesRect.setWidth( myValuesRect.width() - 10 );

    myValuesRect.setX( myOrigin.x() );
    myX = qreal( myValuesRect.width() ) / ( model()->rowCount() );

    setAlphaBeta();

    int bottomMargin = 0;
    if ( myMinBottomMargin < myX ) {
      myVerticalXAxis = false;
      bottomMargin = metrics.height() + 10;
    } else {
      myVerticalXAxis = true;
      bottomMargin = myMinBottomMargin;
    }

    qreal delta = myValuesRect.bottom() - ( myOrigin.y() + bottomMargin );

    if ( delta < 0 ) {
      myValuesRect.setHeight( myValuesRect.height() + delta );
      setAlphaBeta();
    }    
}

QList<int> LinearChart::calculateColumnsOrder() {
  QList<int> bars;
  QList<int> lines;
  QList<int> points;

  for ( int i = 0; i < model()->columnCount(); ++i ) {
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

QList<int> LinearChart::barStyleColumns() const {
    QList<int> bars;
    for ( int c = 0;  c < model()->columnCount(); ++c ) {
      if ( columnType( c ) == Marb::Bar ) {
        bars.append( c );
      }
    }
    return bars;
}


QRectF LinearChart::itemRect( const QModelIndex& index ) const {
    QRect r;
    Marb::Types t = columnType( index.column() );
    QVariant var = index.data();
    if ( !var.isValid() ) {
      return QRect();
    }
    qreal value = var.toReal();
    qreal y = valueToPx(value);
    qreal x = myOrigin.x() + index.row() *  myX;
    qreal space = myX * 0.1;
    if ( t == Marb::Bar ) {
      QList<int> bars = barStyleColumns();
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

void LinearChart::paintChart( QPainter& painter ) {
      painter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );

      paintAxis(painter);
      QList<int> orderedColumns = calculateColumnsOrder();
      foreach( int c, orderedColumns ) {
        paintValues( painter, c );
      }
      paintTextAxis(painter);

      painter.drawText( myTitleRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, myTitle );

      paintLegend( painter );
}

void LinearChart::paintAxis( QPainter& painter) {
    painter.save();
    painter.setPen( QPen( QColor(Marb::LightGray), 1.5 ) );

    paintXAxis(painter);
    paintYAxis(painter);

    painter.restore();
}


void LinearChart::paintXAxis( QPainter& painter ) {
    QPoint p1( myOrigin.x(), myOrigin.y() );
    QPoint p2( myChartRect.topRight().x(), myOrigin.y() );
    painter.drawLine( p1, p2 );

    qreal x = myX + myOrigin.x();
    int i = 0;
    while (i < model()->rowCount() ) {
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

void LinearChart::paintTextAxis( QPainter& painter) {
    painter.save();
    QFontMetrics metrics( font() );
    qreal h = metrics.height();
    QPoint textPos( h/2 , myOrigin.y() + 5 );


    painter.setPen( QPen( QColor(Marb::DarkGray), 1.5 ) );
    if ( myVerticalXAxis == true ) {
      qreal x = myX + myOrigin.x();
      int i = 0;
      while ( i < model()->rowCount() ) {
        QPoint p1( x, myOrigin.y() - 3  );
        QString s = model()->headerData( i, Qt::Vertical ).toString();
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
      while ( i < model()->rowCount() ) {
        QString s = model()->headerData( i, Qt::Vertical ).toString();
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
    Marb::Types t = columnType( column );
    QStyledItemDelegate* delegate = 0;

    if ( t.testFlag( Marb::Point ) ) {
      delegate = myPointDelegate;
    } else if ( t.testFlag( Marb::Bar ) ) {
      delegate = myBarDelegate;
    }

    int rows = model()->rowCount();
    painter.save();
    ChartStyle style = columnStyle( column );
    painter.setBrush( style.brush() );
    painter.setPen( style.pen() );

    if ( t.testFlag( Marb::Line) ) {
      QPolygonF poly;
      for ( int r = 0; r < rows; ++r ) {
        QModelIndex index = model()->index( r, column );
        poly << itemRect( index ).center();
      }
      painter.drawPolyline( poly );
    }

    for ( int r = 0; r < rows; ++r ) {
      QModelIndex index = model()->index( r, column );
      QStyleOptionViewItem option;
      qreal value = index.data().toReal();
      if ( value < 0 ) {
        option.decorationPosition = QStyleOptionViewItem::Bottom;
      } else {
        option.decorationPosition = QStyleOptionViewItem::Top;
      }

      option.rect = itemRect( index ).toRect();

      if ( delegate != 0 ) {
        delegate->paint( &painter, option, index );
      }
    }
    painter.restore();
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
      myPointDelegate->paint( &painter, option, model()->index( 0, column ) );
    }
    painter.restore();
}
