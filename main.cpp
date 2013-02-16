#include <QApplication>

#include <QStandardItemModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QTreeView>
#include <QDebug>
#include <QDateTime>
#include <QPushButton>
#include <QScrollBar>


#include "charts/linearchart.h"
#include "charts/radialchart.h"
#include "charts/piechart.h"
#include "charts/piechart3D.h"

int main( int argc, char* argv[] ) {
  QApplication a(argc, argv);

  QStandardItemModel* model = new QStandardItemModel(5,5);
  model->setHeaderData(0, Qt::Horizontal, ("Label"));
  model->setHeaderData(1, Qt::Horizontal, ("Value"));
  model->setVerticalHeaderLabels( QStringList() << "test" << "test" );
  qsrand( QDateTime::currentDateTime().toTime_t() );
  for (int i = 0; i < model->rowCount(); ++i ) {
    qreal v = (qrand() % ( 30 - 10) + 10);
    model->setData( model->index( i, 0 ), v, Qt::DisplayRole );
    v = (qrand() % ( 25));
    model->setData( model->index( i, 1 ), v, Qt::DisplayRole );
    model->setData( model->index( i, 1 ), i * 10, Qt::DisplayRole );
    model->setData( model->index( i, 2 ), i * 10, Qt::DisplayRole );
    model->setData( model->index( i, 3 ), i * 10, Qt::DisplayRole );
    model->setData( model->index( i, 4 ), i * 10, Qt::DisplayRole );
  }

  /*QColor c( 50, 50, 90 );
  for (int i = 0; i < model->rowCount(); ++i ) {
    model->setData( model->index( i, 0 ), c, Qt::DecorationRole );
    c = c.lighter( 120 );
  }*/

  //model->setData( model->index( 0, 0 ), -24, Qt::DisplayRole );
  //model->setData( model->index( 1, 0 ), 80, Qt::DisplayRole );

  LinearChart* linear = new LinearChart();
  linear->setModel( model );
  linear->setDataStyle( 1, LinearChart::Bar );
  linear->setDataStyle( 0, LinearChart::Line );

  RadialChart* chart = new RadialChart();
  chart->setModel( model );

  /*QWidget w;
  QHBoxLayout layout( &w );
  layout.addWidget( chart );
  layout.addWidget( linear );
  w.showMaximized();*/
  //chart->setFixedSize( 500, 500 );
  //chart->show();
  chart->resize( 500, 500 );
  //chart->setColumnVisible( 1, false );
  //linear->show();

  PieChart pie;
  pie.setModel( model );
  pie.setLegend( "An example of a pie chart" );
  pie.show();
  pie.setRing( true );
  pie.setGeometry(0, 100, 500, 500 );
  pie.setSplitted( true );
  QFont f("Helvetica [Cronyx]", 20);

  f.setBold( true );
  pie.setFont( f );
  pie.update();

  PieChart ring;
  ring.setModel( model );
  //ring.setRing( true );
  ring.show();
  ring.setGeometry( 500, 100, 500, 500 );

  PieChart3D pie3D;
  pie3D.setModel( model );
  pie3D.show();
  pie3D.setGeometry(1000, 100, 500, 500 );

  QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
  pie.setSelectionModel(selectionModel);
  ring.setSelectionModel(selectionModel);


  return a.exec();
}
