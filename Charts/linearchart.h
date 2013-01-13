#ifndef LINEARCHART_H
#define LINEARCHART_H

#include "chart.h"

class LinearChart : public Chart {
  Q_OBJECT
public:
  enum Style {
    Point  = 0,
    Line   = 1,
    Bar    = 2
  };
  Q_ENUMS( Style )
public:
  explicit LinearChart(QWidget *parent = 0);
  QModelIndex indexAt(const QPoint &point) const;
  void     showData( int column, LinearChart::Style style = LinearChart::Point );
signals:
  
public slots:
  
};

#endif // LINEARCHART_H
