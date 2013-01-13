#ifndef CHARTSPEC_H
#define CHARTSPEC_H
#include <QAbstractItemModel>
#include <QSize>
#include <QPoint>

class ChartSpec {
public:
  ChartSpec( QAbstractItemModel* model = 0 );
  QAbstractItemModel* model;
  qreal  m;
  qreal  p;
  qreal  nbStep;
  qreal  yScale;
  qreal  xScale;
  qreal  yOrder;
  qreal  yStep;
  QPoint chartPos;
  QSize  chartSize;
  qreal  max;
  qreal  min;
  qreal  yLabelsLength;
  qreal  xLabelsLength;
  QList<int> usedColumns;

  qreal order( qreal v );
  void  calculate( QList<int> usedColumns );
  qreal gridStartValue();
  void  xAxisSpace();
  void  yAxisSpace();
  qreal valueToPixel( qreal );
};

#endif // CHARTSPEC_H
