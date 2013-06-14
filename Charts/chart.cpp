#include "chart.h"

#include <QFontMetrics>
#include <qmath.h>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QDebug>

Chart::Chart( QWidget* parent ) : AbstractMarbView( parent ) {
  myMin = 0;
  myMax = 0;
  myMinBound = 0;
  myMaxBound = 0;
  myOrder = 1;
  myNbTicks = 10;
  myTickSize = 0;
  myAlpha = 1;
  myBeta = 1;
  myChartRect = QRect();
  myValuesRect = QRect();
  myTitleRect = QRect();
  myTitle = "";
  myTitleRect = QRect();
  myOrigin = QPoint(0,0);
  myNbDigits = 1;
  myMarginX = 20;
  myMarginY = 20;
}

void Chart::resizeEvent(QResizeEvent * ev) {
  QAbstractItemView::resizeEvent( ev );
  processSpec();
}

void Chart::setModel( QAbstractItemModel* model ) {
  QAbstractItemView::setModel( model );
  process();
}

void Chart::process() {
  if ( model() == 0 ) {
    return;
  }
  myMin = 0;
  myMax = 0;

  myMinBottomMargin = scanValues() + 10.0;

  processSpec();
}

Chart::~Chart() {

}

void Chart::setTitle( QString title ) {
  myTitle = title;
  process();
}

int Chart::scanValues() {
  int rows = model()->rowCount();
  int cols = model()->columnCount();

  QFontMetrics metrics( font() );
  int textWidth = 0;
  for (int r = 0; r < rows; ++r ) {
    QString s = model()->headerData( r, Qt::Vertical ).toString();
    textWidth = qMax( textWidth, metrics.width( s ) + 5 );
    for (int c = 0; c < cols; ++c ) {
      qreal value = model()->index( r, c ).data().toReal();
      myMin = qMin( myMin, value );
      myMax = qMax( myMax, value );
    }
  }
  return textWidth;
}

void Chart::calculateBounds() {
  myMinBound = myMin;
  myMaxBound = myMax;

  if ( myMaxBound == myMinBound ) {
    ++myMaxBound;
    --myMinBound;
  }

  myOrder = calculateOrder( myMax - myMin );
  myTickSize = (myMax - myMin ) / (myNbTicks - 1);

  if ( myOrder >= 10 ) {
    myNbDigits = 0;
  } else if ( myOrder == 1 ) {
    myNbDigits = 2;
  } else {
    int nbZero = QString::number(myOrder).count( "0" );
    myNbDigits = nbZero + 2;
  }
}

void Chart::calculateLegendRect() {
    QFontMetrics metrics( font() );
    int h = metrics.height() + 5;
    int cols = model()->columnCount();
    int nbLines = 1;
    int w = 50;
    int maxWidth = myChartRect.width();
    for( int c = 0; c < cols; ++c ) {
      QString s( model()->headerData( c, Qt::Horizontal ).toString() );
      int sWidth = metrics.width( s ) + 50;
      if ( ( w + sWidth ) > maxWidth ) {
        ++nbLines;
        w = sWidth;
      } else {
        w += sWidth;
      }
    }
    myLegendRect = QRect( myMarginX, myMarginY, maxWidth, nbLines * h );
}

qreal Chart::calculateOrder( qreal value ) const {
  qreal order = 1.0;
  qreal v = qAbs( value );
  if ( v >= 1 ) {
    while ( v > 1 ) {
      order *= 10.0;
      v /= 10.0;
    }
    order /= 10.0;

  } else if ( v != 0 ) {
    while ( v < 1 ) {
      order /= 10.0;
      v *= 10.0;
    }
    order *= 10.0;
  }

  return order;
}

void Chart::setAlphaBeta() {
  myAlpha = -qreal(myValuesRect.height()) / ( myMaxBound - myMinBound );
  myBeta = (myMaxBound * myValuesRect.height() ) / ( myMaxBound - myMinBound ) + myValuesRect.y();
  myOrigin.setY( myBeta );
}

qreal Chart::valueToPx( qreal value) const {
  return value * myAlpha + myBeta;
}

ChartStyle Chart::columnStyle( int column ) const {
  if ( myStyle.contains( column ) ) {
      return myStyle[ column ];
  }
  ChartStyle style;
  QColor c1 =Marb::predefinedLightColor( column );
  QColor c2 = Marb::predefinedDarkColor( column );
  style.setPen( QPen( QColor(c2), 2 ) );
  style.setBrush( QBrush(c1) );
  return style;
}

void Chart::setColumnStyle( int column, ChartStyle style ) {
    myStyle[ column ] = style;
}

/*Paint the legend in the QRect self._legendRect

The legend corresponds to the text in the Horizontal QHeaderView and the style defined for each column.
*/
void Chart::paintLegend( QPainter& painter) {
    painter.save();
    QFontMetrics metrics( font() );
    int metricsH = metrics.height();
    int h = metricsH + 5;
    int cols = model()->columnCount();
    int w = 0;
    int maxWidth = myLegendRect.width();

    QPoint legendPos( myLegendRect.topLeft() );
    QPoint pos = legendPos + QPoint( 50, 0);
    for (int c = 0; c < cols; ++c ) {
      QString s( model()->headerData( c, Qt::Horizontal ).toString() );
      int sWidth = metrics.width( s ) + 50;
      QPoint p;
      if ( ( w + sWidth ) > maxWidth ) {
        int y = pos.y();
        p = QPoint( myLegendRect.x() + 10, y + h );
        pos = QPoint( myLegendRect.x() + 50, y + h );
        w = sWidth;
        pos += QPoint( sWidth, 0 );
      } else {
        p = pos + QPoint( -40,  0 );
        w += sWidth;
        pos += QPoint( sWidth, 0 );
      }
      paintColumnLegend(painter, c, p + QPoint(0, metricsH), metricsH);
    }
    painter.restore();
}

void Chart::paintColumnLegend(QPainter &painter, int column, QPoint pos, int maxHeight) {
}

bool Chart::save( QString filename ) {
  QPixmap pix( size() );
  pix.fill( Qt::white );
  QPainter painter( &pix );
  paintChart( painter );
  return pix.save( filename );
}

void Chart::paintEvent( QPaintEvent* event ) {
  if ( model() == 0 ) {
    return;
  }
    QPainter painter( viewport() );
    painter.setClipRect( event->rect() );
    paintChart( painter );
}
