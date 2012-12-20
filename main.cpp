#include <QtGui/QApplication>

#include <QStandardItemModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QTreeView>
#include <QDebug>
#include <QDateTime>
#include <QPushButton>
#include <QScrollBar>


#include "charts/chart.h"
#include "charts/radialchart.h"

int main( int argc, char* argv[] ) {
  QApplication a(argc, argv);

  QStandardItemModel* model = new QStandardItemModel(15,2);
  model->setHeaderData(0, Qt::Horizontal, ("Label"));
  model->setHeaderData(1, Qt::Horizontal, ("Value"));
  model->setVerticalHeaderLabels( QStringList() << "test" << "test" );
  qsrand( QDateTime::currentDateTime().toTime_t() );
  for (int i = 0; i < 15; ++i ) {
    qreal v = i + 1;//(qrand() % ( 50  - 10)) + 10;
    model->setData( model->index( i, 0 ), v, Qt::DisplayRole );
    v = 10;
    model->setData( model->index( i, 1 ), v, Qt::DisplayRole );
  }

  RadialChart chart;
  chart.setModel( model );
  Chart chart1;
  chart1.setModel( model );
  chart1.showData( 0, Chart::Bar );
  //chart1.showData( 1, Chart::Line );
  chart.showMaximized();

  return a.exec();
}
