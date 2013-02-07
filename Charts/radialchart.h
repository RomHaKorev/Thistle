#ifndef RADIALCHART_H
#define RADIALCHART_H

#include "pointchart.h"
#include "chartspec.h"

class RadialChart : public PointChart {
  Q_OBJECT

protected:
  QMap<int, QColor> myColors;
  QList<int>        myHiddenColumns;
  QLineF            myLine;
  ChartSpec         mySpec;
  QRect             myRect;
  qreal             myOffset;
  void              updateChart();

public:
  explicit RadialChart( QWidget* parent = 0 );
  void resizeEvent( QResizeEvent* event );
  void paintChart(QPainter& painter);
  void paintAxis( QPainter& painter );
  void paintGrid( QPainter& painter );
  void paintValue( QPainter& painter, QModelIndex& index, qreal delta, qreal angle );
  void paintEvent(QPaintEvent *);
  void setModel(QAbstractItemModel *model);
  void setColumnVisible( int column, bool visible = true );
signals:
  
public slots:
};

#endif // RADIALCHART_H
