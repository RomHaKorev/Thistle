#ifndef LINECHART_H
#define LINECHART_H

#include "pointchart.h"

class LineChart : public PointChart {
  Q_OBJECT

protected:
  virtual void paintChart(QPainter& painter);
public:
  explicit LineChart( QWidget* parent = 0 );
  ~LineChart();
  
};

#endif // LINECHART_H
