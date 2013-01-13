#include "circularchart.h"
#include <QPainter>
#include "pointchart.h"
#include "radialchart.h"
#include <QDebug>
CircularChart::CircularChart( QWidget* parent ) : Chart( parent ) {
}

void CircularChart::showData( int column, CircularChart::Style style ) {
  if ( myModel != 0 && column < myModel->columnCount() ) {
    PointChart* chart;
    if ( style == Radial ) {
      chart = new RadialChart();
    } else {
      chart = new RadialChart();
    qWarning() << "Could not show data : Invalid model or column";
    }
    QPalette p;
    p.setColor( QPalette:: Base, Qt::transparent );
    chart->setPalette( p );
    chart->resize( size() );
    chart->setModel( myModel );
    chart->setSelectionModel( mySelections );
    chart->setColumnData( column );
    chart->move( mySpec.chartPos );
    chart->setObjectName( "Chart " + QString::number( column ) );
    myCharts.insert( column, chart );
    chart->setColor( (Chart::PredefinedColor)(myCharts.count()) );
  } else {
    qWarning() << "Could not show data : Invalid model or column";
  }
}


void CircularChart::resizeEvent( QResizeEvent* ev ) {
  QAbstractItemView::resizeEvent( ev );
  int w = qMin( width() - 40, height() - 40 );
  mySpec.chartSize = QSize( w, w );

  foreach( PointChart* p, myCharts.values() ) {
    p->setFixedSize( mySpec.chartSize );
  }
  updateChart();
}

void CircularChart::paintAxis( QPainter& painter ) {
  painter.save();
  painter.setPen( Qt::black );
  QPoint p1 = mySpec.chartPos + QPoint( mySpec.chartSize.width() / 2, mySpec.chartSize.height() / 2 );
  QPoint p2 = p1 + QPoint( mySpec.chartSize.width() / 2, 2 );
  painter.drawLine( p1, p2 );
  painter.restore();
}

void CircularChart::paintEvent(QPaintEvent *event) {

}
