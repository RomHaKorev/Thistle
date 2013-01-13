#ifndef CIRCULARCHART_H
#define CIRCULARCHART_H

#include "chart.h"

class CircularChart : public Chart {
  Q_OBJECT
public:
  enum Style {
    Radial = 0,
    Pie = 1
  };
  Q_ENUMS( Style )

protected:
  void resizeEvent( QResizeEvent* ev );
  void paintAxis( QPainter& painter );
  void paintEvent( QPaintEvent* event );
public:
  explicit CircularChart( QWidget* parent = 0 );
  void showData( int column, CircularChart::Style style );
};

#endif // CIRCULARCHART_H
