#ifndef RADIALCHART_H
#define RADIALCHART_H

#include "pointchart.h"

class RadialChart : public PointChart {
  Q_OBJECT

protected:
  QRect myRect;
  qreal myM;
  qreal myP;
  void updateChart();
public:
  explicit RadialChart( QWidget *parent = 0 );
  void paintChart(QPainter *painter);
  //void paintAxis( QPainter* painter );
  void paintEvent(QPaintEvent *);
signals:
  
public slots:
};

#endif // RADIALCHART_H
