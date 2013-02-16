#include "chartspec.h"

#include <qmath.h>
#include <QFontMetrics>
#include <QDebug>

ChartSpec::ChartSpec( QAbstractItemModel* m ) {
  model = m;
  yScale = 0;
  xScale = 0;
  yOrder = 0;
  chartSize = QSize( 1, 1 );
  max = 0;
  min = 0;
  yLabelsLength = 0;
  chartPos = QPoint( 20, 20 );
  m = 0;
  p = 0;
  nbStep = 5;
}

void ChartSpec::calculate( QList<int> col ) {
  if ( model == 0 ) {
    return;
  }

  usedColumns = col;

  QModelIndex index = model->index( 0, usedColumns.first() );
  min = 0;
  max = 0;

  foreach( int col, usedColumns ) {
    for ( int r = 0; r < model->rowCount(); ++r ) {
      index = model->index( r, col );
      min = qMin( min, model->data( index ).toReal() );
      max = qMax( max, model->data( index ).toReal() );
    }
  }

  if ( qFuzzyCompare( 1 + qAbs( min ), 1 + qAbs( max ) ) == true ) {
    --min;
    ++max;
  }

  yOrder = order( max - min );

  yStep = (max - min) / nbStep;

  qreal w = chartSize.width();
  xScale = w / model->rowCount();

  xAxisSpace();
  yAxisSpace();

  qreal h = chartSize.height();
  m = -h / ( max - min );
  p = h - m * min;


  chartPos = QPoint( xScale * 0.2 + yLabelsLength, 20 );
}

qreal ChartSpec::gridStartValue() {
  qreal o = order( min );
  qreal i = qAbs( min / o );
  qreal floor = i - qFloor( i );
  i -= floor;
  if ( floor < 0.5 && floor > 0 ) {
    i += 0.5;
  } else if ( floor >= 0.5 ) {
    i += 1;
  }
  if ( min < 0 ) {
   i = -i * o;
  } else {
    i = i * o;
  }
  return i;
}

qreal ChartSpec::order( qreal v ) {
  qreal order = 1;
  v = qAbs( v );
  if ( v >= 1 ) {
    while ( v > 1 ) {
      order *= 10;
      v /= 10;
    }
    order /= 10;
  } else if ( v != 0 ) {
    while ( v < 1 ) {
      order /= 10;
      v *= 10;
    }
    order *= 10;
  }
  return order;
}

void ChartSpec::xAxisSpace() {
  QFont font;
  QFontMetrics metrics( font );
  xLabelsLength = 0;
  for ( int i = 0; i < model->rowCount(); ++i ) {
    QString str = model->headerData( i, Qt::Vertical ).toString();
    xLabelsLength = qMax( xLabelsLength,  (qreal)metrics.width( str ) );
  }

  qreal spaceForValue = 0.2 * chartSize.height();
  spaceForValue = qMin( spaceForValue, xLabelsLength );
}

void ChartSpec::yAxisSpace() {
  QFont font;
  QFontMetrics metrics( font );
  yLabelsLength = qMax( metrics.width( QString::number(order( max )) ),
                        metrics.width( QString::number(order( min )) ) );
  return;
  qreal spaceForValue = 0.2 * chartSize.height();
  spaceForValue = qMin( spaceForValue, xLabelsLength );
}

qreal ChartSpec::valueToPixel( qreal v ) {
  return m * v + p;
}
