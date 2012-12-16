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

int main( int argc, char* argv[] ) {
  QApplication a(argc, argv);

  QStandardItemModel* model = new QStandardItemModel(15,2);
  model->setHeaderData(0, Qt::Horizontal, ("Label"));
  model->setHeaderData(1, Qt::Horizontal, ("Value"));
  qsrand( QDateTime::currentDateTime().toTime_t() );
  for (int i = 0; i < 15; ++i ) {
    qreal v = (qrand() % ( 50  + 49)) -40;
    model->setData( model->index( i, 0 ), v, Qt::DisplayRole );
    v = (qrand() % ( 50  + 49)) -40;
    model->setData( model->index( i, 1 ), v, Qt::DisplayRole );
    //model->appendRow( new QStandardItem( QString::number(v) ) );
  }

  Chart chart;
  chart.setModel( model );
  chart.showData( 0, Chart::Line );
  chart.showData( 1, Chart::Bar );
  chart.show();

  return a.exec();
}
