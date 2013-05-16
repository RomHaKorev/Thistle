#ifndef RADIALCHART_H
#define RADIALCHART_H
#include "chart.h"


class RadialChart: public Chart {
  Q_OBJECT
protected:
  qreal myCenterHoleDiam;

  QRectF itemRect(const QModelIndex &index) const;
  void setAlphaBeta();
  void processSpec();
  void paintTextAxis( QPainter& painter) const;
  void paintValues( QPainter& painter, int column ) const;
  void paintAxis( QPainter& painter ) const;
  void paintTicks( QPainter& painter ) const;
  virtual void paintChart(QPainter &);

public:
  RadialChart( QWidget* parent = 0 );
  void process();
};

#endif //RADIALCHART_H
