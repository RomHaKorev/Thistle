#ifndef BAR_H
#define BAR_H

#include "pointchart.h"

class BarChart : public PointChart {
  Q_OBJECT

protected:
  int my_ratio;
  int my_part;
  virtual void paintChart(QPainter *painter);
public:
  explicit BarChart( QWidget* parent = 0 );
  void setWidthRatio( int ratio, int my_part = 0 );
  ~BarChart();

};

#endif // BAR_H
