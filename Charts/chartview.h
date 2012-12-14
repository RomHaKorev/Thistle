#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QWidget>
#include "pointchart.h"

class ChartView : public QWidget {
  Q_OBJECT
protected:
  qreal my_maximum;
  qreal my_minimum;
  PointChart* my_last;
  QAbstractItemModel* my_model;
  QItemSelectionModel my_selections;
  QMap< int, PointChart* > my_charts;
  void resizeEvent(QResizeEvent * ev);
  void mousePressEvent(QMouseEvent *);
public:
  explicit ChartView( QAbstractItemModel* model, QWidget* parent = 0 );
  void     setModel( QAbstractItemModel* model );

  void     showData( int column );
  void     hideData( int column );
signals:
  
public slots:
  
};

#endif // CHARTVIEW_H
