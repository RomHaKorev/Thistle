#include "linearchart.h"
#include <QDebug>

#include "pointchart.h"
#include "linechart.h"
#include "barchart.h"
#include "radialchart.h"

LinearChart::LinearChart( QWidget* parent ) :
  Chart( parent ) {
}

void LinearChart::showData( int column, LinearChart::Style style ) {
  if ( myModel != 0 && column < myModel->columnCount() ) {
    PointChart* chart;
    if ( style == Point ) {
      chart = new PointChart();
    } else if ( style == Line ) {
      chart = new LineChart();
    } else if ( style == Bar ) {
      qDebug() << "Bar chart";
      int nb = 1;
      foreach( PointChart* p, myCharts ) {
        BarChart* bar = qobject_cast<BarChart*>( p );
        if ( bar != 0 ) {
          ++nb;
        }
      }
      BarChart* bar = new BarChart();
      bar->setWidthRatio( nb, 0 );
      chart = bar;
      int i = 1;
      foreach( PointChart* p, myCharts ) {
        BarChart* bar = qobject_cast<BarChart*>( p );
        if ( bar != 0 ) {
          bar->setWidthRatio( nb, i );
          ++i;
        }
      }
    } else {
      qWarning() << "Chart style not supported";
      chart = new PointChart();
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
  updateChart();
}

QModelIndex LinearChart::indexAt(const QPoint &point) const {
  foreach( PointChart* p, myCharts ) {
    BarChart* b = qobject_cast<BarChart*>(p);
    QModelIndex id;
    if ( b != 0) {
      b->indexAt2( point - mySpec.chartPos );
    }
    if ( id.isValid() ) {
      return id;
    }
  }
  return QModelIndex();
}
