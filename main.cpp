#include <QtGui/QApplication>

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

int main( int argc, char* argv[] ) {
  QApplication a(argc, argv);

  QStandardItemModel* model = new QStandardItemModel(20,5);
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
  pie.show();
  pie.resize( 500, 500 );

  return a.exec();
}
