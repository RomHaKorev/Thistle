#ifndef BAR_H
#define BAR_H

#include "pointchart.h"

class BarChart : public PointChart {
  Q_OBJECT

protected:
  QList<QRect> test;
  QPoint test1;
  int my_ratio;
  int my_part;
  virtual void paintChart(QPainter& painter);
  virtual QModelIndex indexAt(const QPoint &point) const;

public:
  virtual QModelIndex indexAt2(const QPoint &point);
  explicit BarChart( QWidget* parent = 0 );
  void setWidthRatio( int ratio, int my_part = 0 );
  ~BarChart();
};

#endif // BAR_H
