#ifndef RADIALCHART_H
#define RADIALCHART_H

#include "pointchart.h"
#include "chartspec.h"

class RadialChart : public PointChart {
  Q_OBJECT

protected:
  QLineF    myLine;
  ChartSpec mySpec;
  QRect     myRect;
  qreal     myOffset;
  void      updateChart();

public:
  explicit RadialChart( QWidget* parent = 0 );
  void resizeEvent( QResizeEvent* event );
  void paintChart(QPainter& painter);
  void paintAxis( QPainter& painter );
  void paintEvent(QPaintEvent *);
  void setModel(QAbstractItemModel *model);
signals:
  
public slots:
};

#endif // RADIALCHART_H
