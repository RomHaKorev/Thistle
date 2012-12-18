#ifndef RADIALCHART_H
#define RADIALCHART_H

#include "pointchart.h"

class RadialChart : public PointChart {
  Q_OBJECT

protected:
  void updateChart();
  QList<QPainterPath> test;
public:
  explicit RadialChart( QWidget *parent = 0 );
  void paintChart(QPainter *painter);
  void paintAxis( QPainter* painter );
  void paintEvent(QPaintEvent *);
signals:
  
public slots:
  
};

#endif // RADIALCHART_H
