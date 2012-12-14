#include "chartview.h"
#include <QDebug>
#include <QResizeEvent>

ChartView::ChartView( QAbstractItemModel* model, QWidget* parent ) :
  QWidget( parent ), my_selections( model ) {
  my_model = model;
  my_last = 0;
  my_maximum = 0;
  my_minimum = 0;
}

void ChartView::resizeEvent(QResizeEvent * ev) {
  foreach( PointChart* p, my_charts.values() ) {
    p->resize( ev->size() );
  }
}

void ChartView::setModel(QAbstractItemModel* model ) {
  my_model = model;
  foreach( PointChart* p, my_charts.values() ) {
    p->setModel( model );
  }
}

void ChartView::showData( int column ) {
  if ( my_model != 0 && column < my_model->columnCount() ) {
    if ( my_charts.contains( column ) ) {
      my_charts[column]->hide();
      my_charts[column]->deleteLater();
    }
    qDebug() << "ajout Chart";
    PointChart* chart;
    if ( my_last == 0 ) {
      chart = new PointChart( this );
    } else {
      chart = new PointChart( my_last );
    }
    QPalette p;
    p.setColor( QPalette:: Base, Qt::transparent );
    chart->setPalette( p );
    chart->resize( size() );
    chart->setModel( my_model );
    chart->setSelectionModel( &my_selections );
    chart->setColumnData( column );
  } else {
    qWarning() << "Could not show data : Invalid model or column";
  }
}

void ChartView::hideData( int column ) {

}

void ChartView::mousePressEvent( QMouseEvent* ev ) {
}
