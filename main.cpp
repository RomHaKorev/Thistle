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
#include "charts/RadialChart.h"

int main( int argc, char* argv[] ) {
  QApplication a(argc, argv);

  QStandardItemModel* model = new QStandardItemModel(15,2);
  model->setHeaderData(0, Qt::Horizontal, ("Label"));
  model->setHeaderData(1, Qt::Horizontal, ("Value"));
  model->setVerticalHeaderLabels( QStringList() << "test" << "test" );
  qsrand( QDateTime::currentDateTime().toTime_t() );
  for (int i = 0; i < 15; ++i ) {
    qreal v = (qrand() % ( 50  - 10)) + 10;
    model->setData( model->index( i, 0 ), v, Qt::DisplayRole );
    v = 10;
    model->setData( model->index( i, 1 ), v, Qt::DisplayRole );
  }

  model->setData( model->index( 0, 0 ), -500, Qt::DisplayRole );
  model->setData( model->index( 1, 0 ), 80, Qt::DisplayRole );

  LinearChart* linear = new LinearChart();
  linear->setModel( model );
  linear->showData( 0, LinearChart::Line );

  RadialChart* chart = new RadialChart();
  chart->setModel( model );

  QWidget w;
  QHBoxLayout layout( &w );
  layout.addWidget( chart );
  layout.addWidget( linear );
  w.showMaximized();

  return a.exec();
}
