#include "linearchart.h"
#include <QDebug>

#include "pointchart.h"
#include "linechart.h"
#include "barchart.h"
#include "radialchart.h"

LinearChart::LinearChart( QWidget* parent ) :
  Chart( parent ) {
}

void LinearChart::setModel( QAbstractItemModel *model ) {
  Chart::setModel( model );
  if ( myModel != 0 ) {
    for ( int column = 0; column < model->columnCount(); ++column ) {
      PointChart* chart = new PointChart();
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

      chart->setColor( Clint::predefinedColor( column ) );
    }
  }
}

void LinearChart::setDataStyle( int column, LinearChart::Style style ) {
  if ( myModel != 0 && column < myModel->columnCount() ) {
    PointChart* chart;
    if ( style == Point ) {
      chart = new PointChart();
    } else if ( style == Line ) {
      chart = new LineChart();
    } else if ( style == Bar ) {
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
    if ( myCharts.contains( column ) ) {
      QColor c = myCharts.value( column )->color();
      myCharts.value( column )->deleteLater();
      myCharts.insert( column, chart );
      chart->setColor( c );
    } else {
      myCharts.insert( column, chart );
      chart->setColor( Clint::predefinedColor( column ) );
    }
  } else {
    qWarning() << "Could not show data : Invalid model or column";
  }
  updateChart();
}

QModelIndex LinearChart::indexAt(const QPoint &point) const {
  foreach( PointChart* p, myCharts ) {
    QModelIndex id = p->indexAt( point );
    if ( id.isValid() ) {
      return id;
    }
  }
  return QModelIndex();
}
